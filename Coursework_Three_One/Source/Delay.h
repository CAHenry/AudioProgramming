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
{
public:
    Delay (int samplesToAllocate, float defaultDelayTime);
    ~Delay ();

    float readDelayLine (int sampleRate);
    LinearSmoothedValue<float> delayTime;
protected:
    AudioBuffer<float> delayLine;
    int maxDelayLength;
    int delayWriteIndex;
};