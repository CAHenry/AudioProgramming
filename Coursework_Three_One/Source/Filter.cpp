/*
  ==============================================================================

    Filter.cpp
    Created: 17 Jan 2018 11:18:26am
    Author:  Craig

  ==============================================================================
*/

#include "Filter.h"

Filter::Filter()
{
}

Filter::Filter (filterResponse response_, float frequency_)
    :response (response_),
    frequency(frequency_)
{
}

Filter::~Filter ()
{

}

 //Called once per block to prevent coefficients being calculated every sample
void Filter::initialise (int processorSampleRate, int blockSize, float delayRampTimeInSeconds)
{
    sampleRate = processorSampleRate;
    frequency.reset (sampleRate/ static_cast<float> (blockSize), delayRampTimeInSeconds); // Rate of smoothing, set to be called every block.
    setNewCoefficients();
}

void Filter::setResponse (filterResponse newResponse)
{
    response = newResponse;
    setNewCoefficients();
}

void Filter::setResponse (int newResponse)
{
    response = (filterResponse) newResponse;
    setNewCoefficients();
}

void Filter::setFrequency (float newFrequency)
{
    frequency.setValue (newFrequency);
    setNewCoefficients();
}

void Filter::setParameters (filterResponse newResponse, float newFrequency)
{
    response = newResponse;
    frequency.setValue (newFrequency);
    setNewCoefficients();
}

void Filter::update ()
{
    if (frequency.isSmoothing ()) // only update if the frequency response is changing
    {
        setNewCoefficients();
    }
}

String Filter::responseToText (float response)
{
    if (response == 0)
        return String ("Low Pass");
    else if (response == 1)
        return String ("High Pass");
    else if (response == 2)
        return String ("Band Pass");
    else
        return String ("error"); // Should never happen provided slider range is set correctly
}

float Filter::textToResponse (String response)
{
    if (response.compare ("Low Pass"))
        return 0.0f;
    else if (response.compare ("High Pass"))
        return 1.0f;
    else if (response.compare ("Band Pass"))
        return 2.0f;
    else
        return 0.0f; // Should never happen provided slider range is set correctly
}

void Filter::setNewCoefficients ()
{
    switch (response)
    {
    case lowPass:
        coefficients = dsp::IIR::Coefficients<float>::makeFirstOrderLowPass (sampleRate, frequency.getNextValue ());
        break;
    case highPass:
        coefficients = dsp::IIR::Coefficients<float>::makeFirstOrderHighPass (sampleRate, frequency.getNextValue ());
        break;
      case bandPass:
        coefficients = dsp::IIR::Coefficients<float>::makeBandPass (sampleRate, frequency.getNextValue ());
        break;
    }
}
