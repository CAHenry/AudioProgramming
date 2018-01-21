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
    //==============================================================================
    /**
    Filter class. Provides utilities to create a customisable filter. Inherets from
    the dsp::IIR::Filter<float> class.

    see dsp::IIR::Filter<float>
    */
{
public:
    //==============================================================================
    /** Enum describing the different filter reponses */
    typedef enum filterResponse
    {
        lowPass,
        highPass,
        bandPass
    } filterResponse;

    /** Default Constructor */
    Filter ();

    /** Constructor, creats a filter with a default filter response and cutoff
    frequency */
    Filter (filterResponse response_, float frequency_);

    /** Destructor */
    ~Filter ();

    //==============================================================================
    /** Initialises the filter class for a sample rate and block size */
    void initialise (int processorSampleRate, int blockSize, float delayRampTimeInSeconds);

    //==============================================================================
    /** Sets the filter response parameter */
    void setResponse (filterResponse response);

    /** Sets the filter response parameter */
    void setResponse (int response);

    /** Sets the cutoff frequency parameter. Internally this class uses the 
    LinearSmoothedValue juce class. The frequency will ramp up to this value as 
    per the ramp time set in initialisation */
    void setFrequency (float frequency);

    /** Sets the cutoff frequency and filter response parameter. Internally this 
    class uses the LinearSmoothedValue juce class. The delay time will ramp up to
    this value as per the ramp time set in initialisation */
    void setParameters (filterResponse response, float frequency);

    /** Update the coefficients of the filter, should be called once per audio block*/
    void update();

    //==============================================================================
    /** Converts the response parameter 0 - 2 to a String describing the filter
    response */
    static String responseToText (float response);

    /** Converts a String describing the filter response to the response
    parameter 0 - 2 */
    static float textToResponse (String response);

private:

    // Filter parameters
    filterResponse response;
    int sampleRate;
    LinearSmoothedValue<float> frequency;

    /** Called internally to update the inhereted public filter coefficient member */
    void setNewCoefficients ();
};

