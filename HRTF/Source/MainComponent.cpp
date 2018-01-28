/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PinnaFilter.h"
#include "NoiseGenerator.h"
#include "Convolution.h"
#include <random>
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent, public Slider::Listener, public Timer
{
public:
    //==============================================================================
    MainContentComponent()
        :
        elevation(0.0)
    {
        setSize (800, 600);

        // specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);

        addAndMakeVisible (&elevationSlider);
        elevationSlider.setSliderStyle (Slider::LinearBarVertical);
        elevationSlider.setRange (-90.0f, 90.0f);
        elevationSlider.addListener (this);

        startTimer (200);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        DBG (String (sampleRate));

        elevation.reset (sampleRate, 0.1);
        convL.prepare (impulseL, convLineL);
        convR.prepare (impulseR, convLineR);
        convLineL.clear ();
        convLineR.clear ();
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!

        // For more details, see the help for AudioProcessor::getNextAudioBlock()


        for (int channel = 0; channel < bufferToFill.buffer->getNumChannels (); ++channel)
        {
            // Get a pointer to the start sample in the buffer for this audio output channel
            float* const buffer = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);

            // Fill the required number of samples with noise betweem -0.125 and +0.125
            for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                buffer[sample] = noiseGen.generateNoise ();
            }
        }


        for (int channel = 0; channel < bufferToFill.buffer->getNumChannels (); ++channel)
        {
            // Get a pointer to the start sample in the buffer for this audio output channel
            float* const buffer = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);
            float* conv = convLines[channel]->getWritePointer (0, 0);
            float* imp = impulses[channel]->getWritePointer (channel, 0);
            // Fill the required number of samples with noise betweem -0.125 and +0.125
            for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                //convs[channel]->convolve (&buffer[sample], conv, imp);

                filters[channel]->calculateCoefficients (elevation.getNextValue());
                filters[channel]->processSample (&buffer[sample]);
            }
        }
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

        // You can add your drawing code here!
    }

    void resized() override
    {
        elevationSlider.setBounds (getLocalBounds ());
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }

    void sliderValueChanged (Slider* slider) override
    {
        elevation.setValue (slider->getValue ());
    }
    
    void timerCallback () override
    {
        noiseGen.bursts ();
    }
private:
    //==============================================================================
    NoiseGenerator noiseGen;

    Slider elevationSlider;
    LinearSmoothedValue<float> elevation;

    PinnaFilter filterL, filterR;
    PinnaFilter* filters[2] = {&filterL, &filterR};
    int count = 0;

    AudioBuffer<float> impulseL, convLineL, impulseR, convLineR;
    AudioBuffer<float>* impulses[2] = {&impulseL, &impulseR};
    AudioBuffer<float>* convLines[2] = {&convLineL, &convLineR};
    Convolution convL, convR;
    Convolution* convs[2] = {&convL, &convR};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
