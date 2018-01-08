/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
DelayAudioProcessorEditor::DelayAudioProcessorEditor (DelayAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p),
    processor (p),
    valueTreeState (vts)
{
    mixLabel.setText ("Mix", dontSendNotification);
    addAndMakeVisible (mixLabel);
    addAndMakeVisible (mixSlider);
    mixAttachment = new SliderAttachment (valueTreeState, "mix", mixSlider);

    feedbackLabel.setText ("Feedback", dontSendNotification);
    addAndMakeVisible (feedbackLabel);
    addAndMakeVisible (feedbackSlider);
    feedbackAttachment = new SliderAttachment (valueTreeState, "feedback", feedbackSlider);

    timeLabel.setText ("Time", dontSendNotification);
    addAndMakeVisible (timeLabel);
    addAndMakeVisible (timeSlider);
    timeAttachment = new SliderAttachment (valueTreeState, "time", timeSlider);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

DelayAudioProcessorEditor::~DelayAudioProcessorEditor()
{
}

//==============================================================================
void DelayAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void DelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    Rectangle<int> r = getLocalBounds ();

    {
        Rectangle<int> mixRect = r.removeFromTop (paramControlHeight);
        mixLabel.setBounds (mixRect.removeFromLeft (paramLabelWidth));
        mixSlider.setBounds (mixRect);

        Rectangle<int> feedbackRect = r.removeFromTop (paramControlHeight);
        feedbackLabel.setBounds (feedbackRect.removeFromLeft (paramLabelWidth));
        feedbackSlider.setBounds (feedbackRect);

        Rectangle<int> timeRect = r.removeFromTop (paramControlHeight);
        timeLabel.setBounds (timeRect.removeFromLeft (paramLabelWidth));
        timeSlider.setBounds (timeRect);
    }
}
