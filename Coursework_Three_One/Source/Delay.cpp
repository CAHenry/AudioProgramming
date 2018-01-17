/*
  ==============================================================================

    Delay.cpp
    Created: 17 Jan 2018 11:18:19am
    Author:  Craig

  ==============================================================================
*/

#include "Delay.h"

Delay::Delay (int samplesToAllocate, float defaultDelayTime)
    : delayLine (1, samplesToAllocate),
    delayTime (defaultDelayTime)
{
    delayWriteIndex = 0;
    maxDelayLength = samplesToAllocate - 1;
}

Delay::~Delay ()
  {
  }

float Delay::readDelayLine (int sampleRate)
{
    const float* delayBuffer = delayLine.getReadPointer (0, 0);

    float delayInSamples = delayTime.getNextValue () * 48000;

    int low = floor (delayInSamples);
    int high = (low + 1) % maxDelayLength;
    float fPart = delayInSamples - low;
    float lowVal = delayBuffer[(delayWriteIndex - low + maxDelayLength) % maxDelayLength];
    float highVal = delayBuffer[(delayWriteIndex - high + maxDelayLength) % maxDelayLength];
    return (lowVal * (1 - fPart)) + (highVal * fPart);
}
