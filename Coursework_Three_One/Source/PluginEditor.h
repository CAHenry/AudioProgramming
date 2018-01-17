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
class DelayAudioProcessorEditor  : public AudioProcessorEditor, private AudioProcessorValueTreeState::Listener
{
public:
    DelayAudioProcessorEditor (DelayAudioProcessor&, AudioProcessorValueTreeState&);
    ~DelayAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    void parameterChanged (const String &parameterID, float newValue);

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayAudioProcessor& processor;

    enum
    {
        paramControlHeight = 40,
        paramLabelWidth = 80,
        paramSliderWidth = 300
    };

    AudioProcessorValueTreeState& valueTreeState;

    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    Label mixLabel;
    Slider mixSlider;
    ScopedPointer<SliderAttachment> mixAttachment;

    Label feedbackLabel;
    Slider feedbackSlider;
    ScopedPointer<SliderAttachment> feedbackAttachment;

    Label timeLabel;
    Slider timeSlider;
    ScopedPointer<SliderAttachment> timeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayAudioProcessorEditor)
};
