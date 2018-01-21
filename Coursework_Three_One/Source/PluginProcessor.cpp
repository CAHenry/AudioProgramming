/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
DelayAudioProcessor::DelayAudioProcessor ()
#ifndef JucePlugin_PreferredChannelConfigurations
        :  AudioProcessor (BusesProperties ()
                        #if ! JucePlugin_IsMidiEffect
                        #if ! JucePlugin_IsSynth
                                .withInput ("Input", AudioChannelSet::stereo (), true)
                        #endif
                                .withOutput ("Output", AudioChannelSet::stereo (), true)
                        #endif
                            ),
            #endif
            parameters (*this, nullptr),
            mix (0.5f),
            feedback (0.8f),
            delay (0.4f, 2.0f),
            filterL (Filter::lowPass, 2000),
            filterR (Filter::lowPass, 2000)
{
    parameters.createAndAddParameter ("mix", "Mix", String (), NormalisableRange<float> (0.0f, 1.0f), 0.5f, nullptr, nullptr);
    parameters.addParameterListener ("mix", this);
    parameters.createAndAddParameter ("feedback", "Feedback", String (), NormalisableRange<float> (0.0f, 1.0f), 0.8f, nullptr, nullptr);
    parameters.addParameterListener ("feedback", this);
    parameters.createAndAddParameter ("time", "Time", String (), NormalisableRange<float> (0.01f, 1.0f), 0.4f, nullptr, nullptr);
    parameters.addParameterListener ("time", this);
    parameters.createAndAddParameter ("syncTime", "Sync Time", String (), NormalisableRange<float> (0.0f, 6.0f), 0.0f, nullptr, nullptr);
    parameters.addParameterListener ("syncTime", this);
    parameters.createAndAddParameter ("sync", "Sync", String (), NormalisableRange<float> (0.0f, 1.0f), 0.0f, nullptr, nullptr);
    parameters.addParameterListener ("sync", this);
    parameters.createAndAddParameter ("response", "Filter Respose", String (), NormalisableRange<float> (0.0f, 2.0f), 0.0f, nullptr , nullptr);
    parameters.addParameterListener ("response", this);
    parameters.createAndAddParameter ("frequency", "Frequency", String (), NormalisableRange<float> (20.0, 20000.0), 2000.0, nullptr, nullptr);
    parameters.addParameterListener ("frequency", this);
    parameters.state = ValueTree (Identifier ("Delay"));
}

DelayAudioProcessor::~DelayAudioProcessor()
{
}

//==============================================================================
const String DelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayAudioProcessor::setCurrentProgram (int index)
{
}

const String DelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    

    mix.reset (sampleRate, 0.1);
    feedback.reset (sampleRate, 0.1);
    delay.initialise(delayLine, getNumInputChannels(), sampleRate, 0.1);
    filterL.initialise (sampleRate, getBlockSize ());
    filterR.initialise (sampleRate, getBlockSize ());
    auto channels = static_cast<uint32> (jmin (getMainBusNumInputChannels (), getMainBusNumOutputChannels ()));

}

void DelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void DelayAudioProcessor::reset ()
{
    filterL.reset();
    filterR.reset ();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DelayAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples ();

    bool sync = *parameters.getRawParameterValue ("sync");
    delay.setSync (sync);

    playHead = this->getPlayHead ();
    playHead->getCurrentPosition (currentPositionInfo);
    delay.setBPM (delayLine, currentPositionInfo.bpm);

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear (i, 0, numSamples);
        filterStereo[i]->update();
    }
    float* channelBuffer[2] = {buffer.getWritePointer (0), buffer.getWritePointer (1)};
    float* delayBuffer[2] = {delayLine.getWritePointer (0), delayLine.getWritePointer (1)};

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...


    // sample iterator then channel iterator due to smoothedValues

    for (int i = 0; i < numSamples; i++)
    {
        float mixVal = mix.getNextValue ();
        float feedbackVal = feedback.getNextValue ();

        float dryMix = 1 - mixVal;
        float wetMix = mixVal;
        for (int channel = 0; channel < totalNumOutputChannels; channel++)
        {
            float in = channelBuffer[channel][i];
            float filteredDelayOutput = filterStereo[channel]->processSample (delay.readSample (delayBuffer[channel]));
            float delayInput = in + (filteredDelayOutput * feedbackVal);
            delay.writeSample (delayBuffer[channel], delayInput);
            channelBuffer[channel][i] = in* dryMix + filteredDelayOutput * wetMix;
        }
        delay.incrementIndex ();
    }
}

//==============================================================================
bool DelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DelayAudioProcessor::createEditor()
{
    return new DelayAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void DelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    ScopedPointer<XmlElement> xml (parameters.state.createXml ());
    copyXmlToBinary (*xml, destData);
}

void DelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr)
        if (xmlState->hasTagName (parameters.state.getType ()))
            parameters.state = ValueTree::fromXml (*xmlState);
}

void DelayAudioProcessor::parameterChanged (const String & parameterID, float newValue)
{
    if (parameterID == "mix")
    {
        mix.setValue (newValue);
    }
    else if (parameterID == "feedback")
    {
        feedback.setValue (newValue);
    }
    else if (parameterID == "syncTime")
    {
        delay.setDelayTime (powf (2.0, newValue) / 16.0);
    }
    else if (parameterID == "time")
    {
        delay.setDelayTime (newValue);
    }
    else if (parameterID == "response")
    {
        filterL.setResponse ((int) newValue);
        filterR.setResponse ((int) newValue);
    }
    else if (parameterID == "frequency")
    {
        filterL.setFrequency (newValue);
        filterR.setFrequency (newValue);
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayAudioProcessor();
}
