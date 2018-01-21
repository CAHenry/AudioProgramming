/*
==============================================================================

This file was auto-generated!

It contains the basic framework code for a JUCE plugin processor.

==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Delay.h"
#include "Filter.h"

class DelayAudioProcessor : public AudioProcessor, public AudioProcessorValueTreeState::Listener
    //==============================================================================
    /**
    Main audio processing class for the delay plugin. The following functions are
    called by the plugin host. They are overridden to create the plugin
    functionality.

    Inherets from juce::AudioProcessor which is general enough
    to be wrapped as a VST, AU, RTAS, etc, or used internally.

    Inherets from juce::AudioProcessorValueTreeState::Listener which provides
    a callback to create custom changes as a result of value tree state parameter
    changes.

    see juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
    */
{
public:
    //==============================================================================
    /** Default Constructor */
    DelayAudioProcessor ();
    /** Destructor. */
    ~DelayAudioProcessor ();

    //==============================================================================
    /** Called before playback starts, to let the processor prepare itself.       */
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;

    /** Called after playback has stopped, to let the object free up any resources
    it no longer needs       */
    void releaseResources () override;

    /** Called by host to stop plugin playing any tails or sound */
    void reset () override;

#ifndef JucePlugin_PreferredChannelConfigurations
    /** Callback to query if the AudioProcessor supports a specific layout */
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif

    /** Process a block of audio, plugin audio processing occurs within */
    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    /** Creates the processor's GUI. */
    AudioProcessorEditor* createEditor () override;

    /** Returns true if the processor has a GUI. */
    bool hasEditor () const override;

    //==============================================================================
    /** returns the Juce_plugin  name */
    const String getName () const override;

    /** Returns true if the plugin accepts midi */
    bool acceptsMidi () const override;

    /** Returns true if the plugin produces midi */
    bool producesMidi () const override;

    /** Returns true if the plugin is a midi effect */
    bool isMidiEffect () const override;

    /** Returns the taillength of the plugin */
    double getTailLengthSeconds () const override;

    //==============================================================================
    /** Returns number of programs */
    int getNumPrograms () override;

    /** Gets the current program */
    int getCurrentProgram () override;

    /** Sets the current program */
    void setCurrentProgram (int index) override;

    /** Returns the program name */
    const String getProgramName (int index) override;

    /** Set the program name */
    void changeProgramName (int index, const String& newName) override;
    //==============================================================================
    /** The host will call this method when it wants to save the processor's
    internal state. */
    void getStateInformation (MemoryBlock& destData) override;

    /** The host will call this method when it wants to recall the processor's
    previous state. */
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Plugin specific public parameters
    LinearSmoothedValue<float> mix;
    LinearSmoothedValue<float> feedback;

    // Plugin processing classes
    Delay delay;
    Filter filterL;
    Filter filterR;
    Filter* filterStereo[2] = {&filterL, &filterR};
private:

    /** AudioProcessorValueTreeState callback. Called when parameters are changed */
    void parameterChanged (const String &parameterID, float newValue);

    // Value tree to hold audio processor parameters
    AudioProcessorValueTreeState parameters;

    // Delay line buffer to be used by the delay class
    AudioBuffer<float> delayLine;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayAudioProcessor)
};
