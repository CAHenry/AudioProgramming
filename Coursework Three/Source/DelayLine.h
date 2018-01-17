/*
  ==============================================================================

    DelayLine.h
    Created: 9 Jan 2018 10:41:34pm
    Author:  Craig

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Delay
{
public:
    Delay (int numSamplesToAllocate);
    ~Delay ();

    void write (float val);
    float read (int tap);
    void increment ();

protected:
    AudioBuffer<float> delayLine;
    int samplesAllocated;
    std::atomic<int> writeIndex;
    LinearSmoothedValue<float> delayTimes[4];
};