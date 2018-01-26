/*
  ==============================================================================

    PinnaFilter.h
    Created: 24 Jan 2018 3:11:17pm
    Author:  Craig

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PinnaEvent.h"

class PinnaFilter
{
public:
    PinnaFilter ();
    ~PinnaFilter ();

    void calculateCoefficients (float elevation);
    void processSample (float* input);

private:
    PinnaEvent zero;
    PinnaEvent one;
    PinnaEvent two;
    PinnaEvent three;
    PinnaEvent four;
    PinnaEvent five;

    PinnaEvent* events[6];

    static const int filterOrder = 31;
    int index = 0;
    float delayLine[filterOrder + 1] = {0};
    float coefficients[filterOrder + 1] = {0};
};