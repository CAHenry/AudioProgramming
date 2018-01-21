/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "CustomSlider.h"

//==============================================================================
/**
*/
class DelayAudioProcessorEditor  : public AudioProcessorEditor, public Timer
{
public:
    DelayAudioProcessorEditor (DelayAudioProcessor&, AudioProcessorValueTreeState&);
    ~DelayAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:

    void timerCallback () override;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayAudioProcessor& processor;

    enum
    {
        paramControlHeight = 80,
        paramLabelWidth = 80,
        paramSliderWidth = 80
    };

    AudioProcessorValueTreeState& valueTreeState;

    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

    Label mixLabel;
    Slider mixSlider;
    ScopedPointer<SliderAttachment> mixAttachment;

    Label feedbackLabel;
    Slider feedbackSlider;
    ScopedPointer<SliderAttachment> feedbackAttachment;

    Label timeLabel;
    TextParameterSlider timeSlider;
    ScopedPointer<SliderAttachment> timeAttachment;

    Label syncTimeLabel;
    TextParameterSlider syncTimeSlider;
    ScopedPointer<SliderAttachment> syncTimeAttachment;

    Label syncLabel;
    ToggleButton syncButton;
    ScopedPointer<ButtonAttachment> syncAttachment;

    Label responseLabel;
    TextParameterSlider responseSlider;
    ScopedPointer<SliderAttachment> responseAttachment;

    Label frequencyLabel;
    Slider frequencySlider;
    ScopedPointer<SliderAttachment> frequencyAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayAudioProcessorEditor)
};
