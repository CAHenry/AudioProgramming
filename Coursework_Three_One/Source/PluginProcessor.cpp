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
    //set defaults for linearsmoothedvalues, delay and filters
            mix (0.5f),
            feedback (0.5f),
            delay (0.5f, 1.0f),
            filterL (Filter::lowPass, 2000),
            filterR (Filter::lowPass, 2000),
            parameters (*this, nullptr)
{
    parameters.createAndAddParameter ("mix", "Mix", String (), NormalisableRange<float> (0.0f, 100.0f), 50.0f, nullptr, nullptr);
    parameters.addParameterListener ("mix", this);
    parameters.createAndAddParameter ("feedback", "Feedback", String (), NormalisableRange<float> (0.0f, 100.0f), 50.0f, nullptr, nullptr);
    parameters.addParameterListener ("feedback", this);
    parameters.createAndAddParameter ("time", "Time", String (), NormalisableRange<float> (0.01f, 1.0f), 0.5f, nullptr, nullptr);
    parameters.addParameterListener ("time", this);
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

int DelayAudioProcessor::getCurrentProgram ()
{
    return 0;
}

void DelayAudioProcessor::setCurrentProgram (int index)
{
}

const String DelayAudioProcessor::getProgramName (int index)
{
    return String ();
}

void DelayAudioProcessor::changeProgramName (int index, const String & newName)
{
}

//==============================================================================
void DelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    auto channels = static_cast<uint32> (jmin (getMainBusNumInputChannels (), getMainBusNumOutputChannels ()));
    mix.reset (sampleRate, 0.1);
    feedback.reset (sampleRate, 0.1);
    delay.initialise(delayLine, channels, static_cast<int>(sampleRate), 0.1f);
    filterL.initialise (static_cast<int>(sampleRate), getBlockSize (), 0.1f);
    filterR.initialise (static_cast<int>(sampleRate), getBlockSize (), 0.1f);

}

void DelayAudioProcessor::releaseResources()
{
    delayLine.setSize (0, 0);
}

void DelayAudioProcessor::reset ()
{
    filterL.reset ();
    filterR.reset ();
    delayLine.clear ();
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

    // clears any extraneous output channels
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear (i, 0, numSamples); 
    }

    // updates the filter coefficients (only once per block for efficiency)
    for (int i = 0; i < totalNumOutputChannels; i++)
    {
        filterStereo[i]->update ();
    }

    // get channel/delay write pointers
    float* channelBuffer[2] = {buffer.getWritePointer (0), buffer.getWritePointer (1)};
    float* delayBuffer[2] = {delayLine.getWritePointer (0), delayLine.getWritePointer (1)};

    for (int i = 0; i < numSamples; i++)
    {
        //update mix and feedback parameters
        float mixVal = mix.getNextValue ();
        float feedbackVal = feedback.getNextValue ();
        float dryMix = 1 - mixVal;
        float wetMix = mixVal;

        //actual delay processing
        for (int channel = 0; channel < totalNumOutputChannels; channel++)
        {
            float in = channelBuffer[channel][i]; // input
            float filteredDelayOutput = filterStereo[channel]->processSample (delay.readSample (delayBuffer[channel])); // delayOutput
            float delayInput = in + (filteredDelayOutput * feedbackVal); // delayInput  filteredDelay * feedback + input
            delay.writeSample (delayBuffer[channel], delayInput); // write delay input to delayLine
            channelBuffer[channel][i] = in* dryMix + filteredDelayOutput * wetMix; // output
        }
        delay.incrementIndex (); // moves to the next frame (and updates delayTime value internally)
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
        mix.setValue (newValue / 100.0f);
    }
    else if (parameterID == "feedback")
    {
        float test = newValue / 100.0f;
        feedback.setValue (newValue / 100.0f);
    }
    else if (parameterID == "time")
    {
        delay.setDelayTime (newValue);
    }
    else if (parameterID == "response")
    {
        filterL.setResponse (static_cast<int> (newValue));
        filterR.setResponse (static_cast<int> (newValue));
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
