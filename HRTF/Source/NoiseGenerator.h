/*
  ==============================================================================

    NoiseGenerator.h
    Created: 27 Jan 2018 11:26:38am
    Author:  Craig

  ==============================================================================
*/

#pragma once
#include <random>

class NoiseGenerator
{
public:
    NoiseGenerator ();

    float generateNoise ();
    void bursts ();

private:

    bool playing = false;
    std::default_random_engine generator;
    std::normal_distribution<float> gaussian;
    int count = 0;
};