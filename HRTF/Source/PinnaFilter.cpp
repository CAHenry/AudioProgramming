/*
  ==============================================================================

    PinnaFilter.cpp
    Created: 24 Jan 2018 3:11:17pm
    Author:  Craig

  ==============================================================================
*/

#include "PinnaFilter.h"

PinnaFilter::PinnaFilter ()
    :
    zero(0),
    one(1),
    two(2),
    three(3),
    four(4),
    five(5),
    events {&zero, &one, &two, &three, &four, &five}
{
}

PinnaFilter::~PinnaFilter ()
{
}


void PinnaFilter::processSample (float* input)
{
    delayLine[index % filterOrder] = *input;
    input[0] = 0;
    for (int i = 0; i < filterOrder; i++)
    {
        input[0] += delayLine[(index + i + 1) % filterOrder] * coefficients[filterOrder - i];
    }

    index = (index + 1) % filterOrder;
}


void PinnaFilter::calculateCoefficients(float elevation)
{
    for (int i = 0; i < filterOrder; i++)
    {
        coefficients[i] = 0;
    }

    for (int n = 0; n < 6; n++)
    {
        float timeDelay = events[n]->getTimeDelay (elevation);
        float reflection = events[n]->getReflectionCoefficient();
        int low = floor (timeDelay);
        int high = low + 1;
        float fPart = timeDelay - low;

        coefficients[low] += reflection * (1.0f - fPart);
        coefficients[high] += reflection * fPart;
    }
}

