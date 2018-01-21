/*
  ==============================================================================

    Delay.h
    Created: 17 Jan 2018 11:18:19am
    Author:  Craig

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Delay
    //==============================================================================
    /**
    Delay class. Provides utilities to create a delay using a juce::AudioBuffer<float>

    see juce::AudioBuffer<float>
    */
{
public:
    //==============================================================================
    /** Constructor, creates a Delay with a given default and maximum delay time */
    Delay (float defaultDelayTime, float maximumDelayTime);

    /** Destructor */
    ~Delay ();

    //==============================================================================
    /** Initialises the delay class for a given buffer, number of channels, 
    sample rate and delay time parameter ramp speed */
    void initialise (AudioBuffer<float> &delayLine, int numberOfChannels, int processorSampleRate, float delayRampTimeInSeconds);

    /** Set the delay time for the delay class. Internally this class uses the 
    LinearSmoothedValue juce class. The delay time will ramp up to this value as 
    per the ramp time set in initialisation */
    void setDelayTime (float val);

    //==============================================================================
    /** Returns the output of the delay line*/
    float readSample (float* &delayBuffer);

    /** Write the float val to the input of the delay line */
    void writeSample (float* &delayBuffer, float val);

    /** Increment the read and write position of the delay line. Call once per frame */
    void incrementIndex ();

private:
    // Delay parameters
    int writeIndex;
    LinearSmoothedValue<float> delayTime; //smoothed delay time
    float currentDelay; // updated once per frame from delayTime
    int delayLineLength; // In samples
    float maximumDelayTime; // In seconds
    int numChannels;
    int sampleRate; // In Hz
};
