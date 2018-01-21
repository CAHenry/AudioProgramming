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

void Delay::setSync (bool syncStatus)
{
     sync = syncStatus;
}

bool Delay::getSync ()
{
    return sync;
}

void Delay::setBPM (AudioBuffer<float> &delayLine, float val)
{
    bpm = val;
    if (sync)
    {
        delayLineLength = (int) (4.0 * (60.0 / (float) bpm) * sampleRate);
    }
    else
    {
        delayLineLength = maximumDelayTime * sampleRate;
    }

    if (delayLineLength != delayLine.getNumSamples())
    {
        delayLine.setSize (numChannels, delayLineLength, true, false, false);
    }
}
void Delay::initialise (AudioBuffer<float> &delayLine, int numberOfChannels, int processorSampleRate, float delayRampTimeInSeconds)
{
    numChannels = numberOfChannels;
    sampleRate = processorSampleRate;
    delayLineLength = maximumDelayTime * sampleRate;
    delayLine.setSize (numChannels, delayLineLength, false, false, false);
    delayTime.reset (sampleRate, delayRampTimeInSeconds);
    currentDelay = delayTime.getNextValue();
    delayLine.clear ();
}

float Delay::readSample (float* &delayBuffer)
{
    float delayInSamples;

    if (sync)
    {
        delayInSamples = currentDelay * sampleRate * (60.0 / (float) bpm);
    }
    else
    {
        delayInSamples = currentDelay * sampleRate;
    }

    int low = floor (delayInSamples);
    int high = (low + 1) % delayLineLength;
    float fPart = delayInSamples - low;

    float lowVal = delayBuffer[(writeIndex - low + delayLineLength) % delayLineLength];
    float highVal = delayBuffer[(writeIndex - high + delayLineLength) % delayLineLength];
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

String Delay::syncToText (float val)
{
    if (val == 0)
    {
        return String ("1/16");
    }
    else if (val == 1)
    {
        return String ("1/8");
    }
    else if (val == 2)
    {
        return String ("1/4");
    }
    else if (val == 3)
    {
        return String ("1/2");
    }
    else if (val == 4)
    {
        return String ("1");
    }
    else if (val == 5)
    {
        return String ("2");
    }
    else if (val == 6)
    {
        return String ("4");
    }
    else if (val == 7)
    {
        return String ("8");
    }
}

float Delay::textToSync (String text)
{
    if (text.compare("1/16"))
    {
        return 0;
    }
    else if (text.compare ("1/8"))
    {
        return 1;
    }
    else if (text.compare ("1/4"))
    {
        return 2;
    }
    else if (text.compare ("1/2"))
    {
        return 3;
    }
    else if (text.compare ("1"))
    {
        return 4;
    }
    else if (text.compare ("2"))
    {
        return 5;
    }
    else if (text.compare ("4"))
    {
        return 6;
    }
    else if (text.compare ("8"))
    {
        return 7;
    }
}
