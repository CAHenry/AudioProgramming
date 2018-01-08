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
class AudioPluginTestAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    AudioPluginTestAudioProcessorEditor (AudioPluginTestAudioProcessor&, AudioProcessorValueTreeState& vts);
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
        paramControlHeight = 40,
        paramLabelWidth = 80,
        paramSliderWidth = 300
    };

    AudioProcessorValueTreeState& valueTreeState;

    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    Label gainLabel;
    Slider gainSlider;
    ScopedPointer<SliderAttachment> gainAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginTestAudioProcessorEditor)
};
