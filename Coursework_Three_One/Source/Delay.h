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
    Delay (float defaultDelayTime, float maximumDelayTime);
    ~Delay ();
    void setDelayTime (float val);
    void setSync (bool syncStatus);
    bool getSync ();
    void setBPM (AudioBuffer<float> &delayLine, float val);
    void initialise (AudioBuffer<float> &delayLine, int numberOfChannels, int processorSampleRate, float delayRampTimeInSeconds);
    float readSample (float* &delayBuffer);
    void writeSample (float* &delayBuffer, float val);
    void incrementIndex ();

    static String syncToText (float val);
    static float textToSync (String text);

private:
    bool sync = false;
    int writeIndex;
    float currentDelay;
    float bpm;
    LinearSmoothedValue<float> delayTime;

    int delayLineLength;
    int maximumDelayTime;
    int numChannels;
    int sampleRate;
};