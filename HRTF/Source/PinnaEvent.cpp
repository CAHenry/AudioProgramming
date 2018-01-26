/*
  ==============================================================================

    PinnaEvent.cpp
    Created: 24 Jan 2018 3:16:18pm
    Author:  Craig

  ==============================================================================
*/

#include "PinnaEvent.h"

float PinnaEvent::coefficientTable[6][4]=
{
    {    1, 1, 0,    1},
    {  0.5, 1, 2,    1},
    {   -1, 5, 4,  0.5},
    {  0.5, 5, 4,  0.5},
    {-0.25, 5, 11, 0.5},
    { 0.25, 5, 13, 0.5}
};

PinnaEvent::PinnaEvent (int N)
    :
    eventNumber (N)
{
    for (int i = 0; i < 3; i++)
    {
        pinnaCoeffs[i] = coefficientTable[N][i + 1];
    }
    reflectionCoefficient = coefficientTable[N][0];
}

PinnaEvent::~PinnaEvent ()
{
}

float PinnaEvent::getTimeDelay (float elevation)
{
    return pinnaCoeffs[0] * sin (pinnaCoeffs[2] * (90.0 - elevation)) + pinnaCoeffs[1];
}

float PinnaEvent::getReflectionCoefficient ()
{
    return reflectionCoefficient;
}

void PinnaEvent::setScalingFactor (float D)
{
    pinnaCoeffs[2] = D;
}
