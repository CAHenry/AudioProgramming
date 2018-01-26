/*
  ==============================================================================

    PinnaEvent.h
    Created: 24 Jan 2018 3:16:18pm
    Author:  Craig

  ==============================================================================
*/

#pragma once
#define _USE_MATH_DEFINES
#include <cmath> 


class PinnaEvent
{
public:
    PinnaEvent (int N);
    ~PinnaEvent ();

    float getTimeDelay (float elevation);
    float getReflectionCoefficient ();

    void setScalingFactor (float D);

private:

    static float coefficientTable[6][4];

    int eventNumber;
    float reflectionCoefficient;
    float timeDelay;
    float pinnaCoeffs[3];
};