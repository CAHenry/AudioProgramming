/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "TextParameterSlider.h"

class DelayAudioProcessorEditor  : public AudioProcessorEditor

    //==============================================================================
    /**
    Component which acts as the GUI for an AudioProcessor.

    Derived from juce::AudioProcessorEditor

    see juce::AudioProcessorEditor, juce::AudioProcessorEditorEditor
    */
{
public:
    //==============================================================================
    /** Creates an editor for the specified processor and value tree state*/
    DelayAudioProcessorEditor (DelayAudioProcessor&, AudioProcessorValueTreeState&);

    /** Destructor. */
    ~DelayAudioProcessorEditor();

    //==============================================================================
    /** Called to paint the application window, overriden from Component. g - the 
    graphics context used to paint the main component                             */
    void paint (Graphics& g) override;

    /** Called to paint the application window, overriden from Component. Used to
    set the layout of the GUI.
    see juce::Graphics             */
    void resized() override;
    //==============================================================================

private:

    /** Reference to access the processor from the editor.                        */
    DelayAudioProcessor& processor;

    /** Reference to access the processor value tree state from the editor.       */
    AudioProcessorValueTreeState& valueTreeState;

    /** Define parameter dimensions                                               */
    enum
    {
        paramControlHeight = 80,
        paramLabelWidth = 80,
        paramSliderWidth = 80
    };

    /** Attaches a slider to a parameter in the value tree                       */
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

    /** GUI elements and attachments to parameters                               */
    Label mixLabel;
    Slider mixSlider;
    ScopedPointer<SliderAttachment> mixAttachment;

    Label feedbackLabel;
    Slider feedbackSlider;
    ScopedPointer<SliderAttachment> feedbackAttachment;

    Label timeLabel;
    TextParameterSlider timeSlider;
    ScopedPointer<SliderAttachment> timeAttachment;

    Label responseLabel;
    TextParameterSlider responseSlider;
    ScopedPointer<SliderAttachment> responseAttachment;

    Label frequencyLabel;
    Slider frequencySlider;
    ScopedPointer<SliderAttachment> frequencyAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayAudioProcessorEditor)
};
