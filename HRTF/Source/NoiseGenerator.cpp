/*
  ==============================================================================

    NoiseGenerator.cpp
    Created: 27 Jan 2018 11:26:38am
    Author:  Craig

  ==============================================================================
*/

#include "NoiseGenerator.h"

NoiseGenerator::NoiseGenerator ()
: gaussian (0.0f, 1.0f)
{

}

float NoiseGenerator::generateNoise ()
{
    return gaussian (generator) * playing;
}

void NoiseGenerator::bursts ()
{
    if (count++ % 5)
    {
        //playing = false;
    }
    else
    {
        playing = true;
    }
}
