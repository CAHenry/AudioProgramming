/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
AudioPluginTestAudioProcessor::AudioPluginTestAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
// ADD UNDO SUPPORT HERE, see tutorial
     : parameters (*this, nullptr),
       AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    parameters.createAndAddParameter ("gain", "Gain", String (), NormalisableRange<float> (0.0f, 5.0f), 1.0f, nullptr, nullptr);
    parameters.state = ValueTree (Identifier ("GainPlugin"));
}

AudioPluginTestAudioProcessor::~AudioPluginTestAudioProcessor()
{
}

//==============================================================================
const String AudioPluginTestAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginTestAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginTestAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginTestAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginTestAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginTestAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginTestAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginTestAudioProcessor::setCurrentProgram (int index)
{
}

const String AudioPluginTestAudioProcessor::getProgramName (int index)
{
    return {};
}

void AudioPluginTestAudioProcessor::changeProgramName (int index, const String& newName)
{
}


//==============================================================================
void AudioPluginTestAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void AudioPluginTestAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudioPluginTestAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void AudioPluginTestAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples ());
    }

    const float currentGain = *parameters.getRawParameterValue ("gain");

    if (currentGain == previousGain)
    {
        buffer.applyGain (currentGain);
    }
    else
    {
        // some sort of interpolation might be better?
        buffer.applyGainRamp (0, buffer.getNumSamples (), previousGain, currentGain);
        previousGain = currentGain;
    }

}


//==============================================================================
bool AudioPluginTestAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AudioPluginTestAudioProcessor::createEditor()
{
    return new AudioPluginTestAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void  AudioPluginTestAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    ScopedPointer<XmlElement> xml (parameters.state.createXml ());
    copyXmlToBinary (*xml, destData);
}

void  AudioPluginTestAudioProcessor::setStateInformation (const void* data, int sizeInBytes) 
{
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr)
        if (xmlState->hasTagName (parameters.state.getType ()))
        {
            parameters.state = ValueTree::fromXml (*xmlState);
            parameters.undoManager->clearUndoHistory ();
        }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginTestAudioProcessor();
}
