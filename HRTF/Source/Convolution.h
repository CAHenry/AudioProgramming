/*
  ==============================================================================

    Convolution.h
    Created: 27 Jan 2018 11:25:16am
    Author:  Craig

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Convolution
{
public:
    Convolution ();
    Convolution (String impulseWav, AudioBuffer <float> &impulse, AudioBuffer <float> &delayLine);

    void convolve (float* sample, float* convLine, float* impulse);

    void prepare (AudioBuffer<float>& impulse, AudioBuffer<float>& convLine);
private:
    int index = 0;
    int size;
    File impulseFile;

    AudioFormatManager formatManager;
};