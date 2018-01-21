/*
  ==============================================================================

    Filter.h
    Created: 17 Jan 2018 11:18:26am
    Author:  Craig

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Filter : public  dsp::IIR::Filter<float>
{
public:

    typedef enum filterResponse
    {
        lowPass,
        highPass,
        bandPass
    };

    Filter ();
    Filter (filterResponse response_, float frequency_);
    ~Filter ();

    void initialise (int processorSampleRate, int blockSize);
    //void copyCoefficients (Filter &filterToCopy);
    void setResponse (filterResponse response);
    void setResponse (int response);
    void setFrequency (float frequency);
    void setParameters (filterResponse response, float frequency);
    void update();

    static String responseToText (float response);
    static float textToResponse (String response);
private:
    filterResponse response;
    int sampleRate;
    LinearSmoothedValue<float> frequency;
    void setNewCoefficients ();
};

#pragma once
