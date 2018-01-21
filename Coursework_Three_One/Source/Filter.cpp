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

 
void Filter::initialise (int processorSampleRate, int blockSize, float delayRampTimeInSeconds)
{
    sampleRate = processorSampleRate;
    frequency.reset (sampleRate/blockSize, delayRampTimeInSeconds);
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
    if (frequency.isSmoothing ())
    {
        setNewCoefficients();
    }
}

String Filter::responseToText (float response)
{
    if (response == 0)
        return String ("LPF");
    else if (response == 1)
        return String ("HPF");
    else if (response == 2)
        return String ("BPF");
    else
        return String ("ERR");
}

float Filter::textToResponse (String response)
{
    if (response.compare ("LPF"))
        return 0.0f;
    else if (response.compare ("HPF"))
        return 1.0f;
    else if (response.compare ("BPF"))
        return 2.0f;
    else
        return 0.0f;
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
        // 0.707 to ensure a -3dB point.
        coefficients = dsp::IIR::Coefficients<float>::makeBandPass (sampleRate, frequency.getNextValue ());
        break;
    }
}
