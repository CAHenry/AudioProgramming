/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PinnaFilter.h"
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
        elevation(45.0),
        gaussian (0.0f, 1.0f),
        impulseL ("D:\documents\FinalYear\AudioProgramming\HRTF\3Rooms27SmallRoomL.wav"),
        impulseR ("D:\documents\FinalYear\AudioProgramming\HRTF\3Rooms27SmallRoomR.wav")
    {
        setSize (800, 600);

        // specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);

        addAndMakeVisible (&elevationSlider);
        elevationSlider.setSliderStyle (Slider::LinearBarVertical);
        elevationSlider.setRange (-90.0f, 90.0f);
        elevationSlider.addListener (this);
        startTimer (500);
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
        dsp::ProcessSpec spec = {sampleRate, samplesPerBlockExpected, 2};
        convolutionL.prepare (spec);
        convolutionL.loadImpulseResponse (impulseL,false, false, impulseL.getSize());
        convolutionR.prepare (spec);
        convolutionR.loadImpulseResponse (impulseR, false, false, impulseR.getSize ());
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!

        // For more details, see the help for AudioProcessor::getNextAudioBlock()

        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)

        for (int channel = 0; channel < bufferToFill.buffer->getNumChannels (); ++channel)
        {
            // Get a pointer to the start sample in the buffer for this audio output channel
            float* const buffer = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);

            // Fill the required number of samples with noise betweem -0.125 and +0.125
            for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                filters[channel]->calculateCoefficients (elevation.getNextValue());
                buffer[sample] = gaussian (generator) * play;
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
        if (count++ % 3)
        {
            play = false;
        }
        else
        {
            play = true;
        }
    }

    //virtual bool keyPressed (const KeyPress& key, Component* originatingComponent) override
    //{
    //}

    //virtual bool keyStateChanged (bool isKeyDown, Component* originatingComponent)
    //{
    //    play = isKeyDown;
    //}

private:
    //==============================================================================
    Slider elevationSlider;
    LinearSmoothedValue<float> elevation;
    bool play = true;
    int count = 0;
    Random random;
    PinnaFilter filterL, filterR;
    PinnaFilter* filters[2] = {&filterL, &filterR};
    dsp::Convolution convolutionL, convolutionR;
    File impulseL, impulseR;
    // Your private member variables go here...

    std::default_random_engine generator;
    std::normal_distribution<float> gaussian;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
