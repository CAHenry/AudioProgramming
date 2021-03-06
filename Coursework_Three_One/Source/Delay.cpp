/*
  ==============================================================================

    Delay.cpp
    Created: 17 Jan 2018 11:18:19am
    Author:  Craig

  ==============================================================================
*/

#include "Delay.h"

Delay::Delay (float defaultDelayTime, float maximumDelayTime)
    : delayTime (defaultDelayTime),
    maximumDelayTime(maximumDelayTime)
{
    writeIndex = 0;
}

Delay::~Delay ()
  {
  }

void Delay::setDelayTime (float val)
{
    delayTime.setValue (val);
}

void Delay::initialise (AudioBuffer<float> &delayLine, int numberOfChannels, int processorSampleRate, float delayRampTimeInSeconds)
{
    numChannels = numberOfChannels;
    sampleRate = processorSampleRate;
    delayLineLength = static_cast<int> (maximumDelayTime) * sampleRate; // maximumDelayTime is in seconds, multiplied by sample rate to give length in samples
    delayLine.setSize (numChannels, delayLineLength, false, false, false);
    delayTime.reset (sampleRate, delayRampTimeInSeconds); // LinearSmoothedValue, sets the ramp time and rate at which the values are required
    currentDelay = delayTime.getNextValue(); // Ensure that the currently delay is initialised
    delayLine.clear ();
}

float Delay::readSample (float* &delayBuffer)
{
    float delayInSamples = currentDelay * sampleRate;
  
    // read index is (writeIndex - delayInSamples + delayLineLength) % delayLineLength
    // however this function linearly interpolates to allow float delay times
    int low = static_cast<int> (floor (delayInSamples));
    int high = (low + 1) % delayLineLength;
    float fPart = delayInSamples - low;

    float lowVal = delayBuffer[(writeIndex - low + delayLineLength) % delayLineLength];
    float highVal = delayBuffer[(writeIndex - high + delayLineLength) % delayLineLength];
  
    // interpolate between low and high
    return (lowVal * (1 - fPart)) + (highVal * fPart);
}

void Delay::writeSample (float* &delayBuffer, float val)
{
    delayBuffer[writeIndex] = val;
}

void Delay::incrementIndex ()
{
    currentDelay = delayTime.getNextValue ();
    writeIndex = (writeIndex + 1) % delayLineLength;
}
