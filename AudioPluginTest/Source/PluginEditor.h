/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class AudioPluginTestAudioProcessorEditor  : public AudioProcessorEditor,
                                             public Slider::Listener,
                                             public Timer
{
public:
    AudioPluginTestAudioProcessorEditor (AudioPluginTestAudioProcessor&);
    ~AudioPluginTestAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginTestAudioProcessor& processor;

    enum
    {
        kParamControlHeight = 40,
        kParamLabelWidth = 80,
        kParamSliderWidth = 300
    };

    OwnedArray<Slider> paramSliders;
    OwnedArray<Label> paramLabels;

    void timerCallback () override;
    void sliderValueChanged (Slider* slider) override;
    void sliderDragStarted (Slider* slider) override;
    void sliderDragEnded (Slider* slider) override;
    AudioParameterFloat* AudioPluginTestAudioProcessorEditor::getParameterForSlider (Slider* slider);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginTestAudioProcessorEditor)
};
