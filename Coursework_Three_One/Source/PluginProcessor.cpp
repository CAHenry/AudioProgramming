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
            filter (dsp::IIR::Coefficients<float>::makeFirstOrderLowPass (48000, 2000))
{
  
    parameters.createAndAddParameter ("mix", "Mix", String (), NormalisableRange<float> (0.0f, 1.0f), 0.5f, nullptr, nullptr);
    parameters.createAndAddParameter ("feedback", "Feedback", String (), NormalisableRange<float> (0.0f, 1.0f), 0.8f, nullptr, nullptr);
    parameters.createAndAddParameter ("time", "Time", String (), NormalisableRange<float> (0.01f, 1.0f), 0.4f, nullptr, nullptr);
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
    delay.initialise(delayLine, getNumInputChannels(), sampleRate, 0.1) ;
    auto channels = static_cast<uint32> (jmin (getMainBusNumInputChannels (), getMainBusNumOutputChannels ()));

}

void DelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void DelayAudioProcessor::reset ()
{
    filter.reset ();
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
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, numSamples);

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...

    float* channelBuffer = buffer.getWritePointer (0);
    float* delayBuffer = delayLine.getWritePointer (0);

    // sample iterator then channel iterator due to smoothedValues

    for (int i = 0; i < numSamples; i++)
    {
        float mixVal = mix.getNextValue ();
        float feedbackVal = feedback.getNextValue ();

        float dryMix = 1 - mixVal;
        float wetMix = mixVal;

        float in = channelBuffer[i];
        float delayOutput = delay.read(delayBuffer);
        channelBuffer[i] = channelBuffer[i] * dryMix + delayOutput * wetMix;
        float delayInput = filter.processSample (in + (delayOutput * feedbackVal));
        delay.writeSample(delayBuffer, delayInput);
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

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayAudioProcessor();
}
