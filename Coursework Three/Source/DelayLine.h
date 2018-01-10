/*
  ==============================================================================

    DelayLine.h
    Created: 9 Jan 2018 10:41:34pm
    Author:  Craig

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class DelayLine
{
public:
    DelayLine (int numSamplesToAllocate);
    ~DelayLine ();

    void write (float val);
    float read (int tap);
    void setDelayTime (int tap, float val);

private:

    AudioBuffer<float> buffer;
    int samplesAllocated;
    std::atomic<int> writeIndex;
    std::atomic<int> delayTime[4];
};