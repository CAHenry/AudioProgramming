/*
  ==============================================================================

    DelayLine.cpp
    Created: 9 Jan 2018 10:41:34pm
    Author:  Craig

  ==============================================================================
*/

#include "DelayLine.h"

DelayLine::DelayLine (int numSamplesToAllocate)
    : buffer (1, numSamplesToAllocate)
{
    samplesAllocated = numSamplesToAllocate;
}

DelayLine::~DelayLine ()
{

}

void DelayLine::write (float val)
{
    float* writePointer = buffer.getWritePointer (0, writeIndex);
    *writePointer = val;
    writeIndex = (writeIndex + 1) % samplesAllocated;
}

float DelayLine::read (int tap)
{
    return *buffer.getReadPointer (0, (writeIndex + delayTime[tap]) % samplesAllocated);
}

void DelayLine::setDelayTime (int tap, float val)
{
    delayTime[tap] = val * 48000;
}