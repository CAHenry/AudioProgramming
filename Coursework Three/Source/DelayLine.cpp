/*
  ==============================================================================

    DelayLine.cpp
    Created: 9 Jan 2018 10:41:34pm
    Author:  Craig

  ==============================================================================
*/

#include "DelayLine.h"

Delay::Delay (int numSamplesToAllocate)
    : delayLine (1, numSamplesToAllocate)
{
    samplesAllocated = numSamplesToAllocate;
    delayTimes[0] = 22100;
    writeIndex = 0;
}

Delay::~Delay ()
{

}

void Delay::write (float val)
{
    float* writePointer = delayLine.getWritePointer (0, writeIndex);
    *writePointer = val;
}

void Delay::increment ()
{
    writeIndex = (writeIndex + 1) % samplesAllocated;
}

float Delay::read (int tap)
{
    float delayTime = delayTimes[tap].getNextValue ();

    int low = floor (delayTime);
    int high = (low + 1) % samplesAllocated;
    float fPart = delayTime - low;
    const float* lowIndex = delayLine.getReadPointer (0, (writeIndex - low + samplesAllocated) % samplesAllocated);
    const float* highIndex = delayLine.getReadPointer (0, (writeIndex - high + samplesAllocated) % samplesAllocated);

    return (*lowIndex * (1 - fPart)) + (*highIndex * fPart);
}
