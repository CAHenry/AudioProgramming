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
      // Initialising GUI components
    mixLabel.setText ("Mix", dontSendNotification);
    addAndMakeVisible (mixLabel);
    addAndMakeVisible (mixSlider);
    mixSlider.setSliderStyle (Slider::Rotary);
    mixAttachment = new SliderAttachment (valueTreeState, "mix", mixSlider);

    feedbackLabel.setText ("Feedback", dontSendNotification);
    addAndMakeVisible (feedbackLabel);
    addAndMakeVisible (feedbackSlider);
    feedbackSlider.setSliderStyle (Slider::Rotary);
    feedbackAttachment = new SliderAttachment (valueTreeState, "feedback", feedbackSlider);

    timeLabel.setText ("Time", dontSendNotification);
    addAndMakeVisible (timeLabel);
    addAndMakeVisible (timeSlider);
    timeSlider.setSliderStyle (Slider::Rotary);
    timeAttachment = new SliderAttachment (valueTreeState, "time", timeSlider);

    responseLabel.setText ("Filter Response", dontSendNotification);
    addAndMakeVisible (responseLabel);
    addAndMakeVisible (responseSlider);
    responseSlider.setGetTextFromValueFunction (Filter::responseToText); //replace text and value functions
    responseSlider.setGetValueFromTextFunction (Filter::textToResponse);
    responseSlider.setSliderStyle (Slider::Rotary);
    responseAttachment = new SliderAttachment (valueTreeState, "response", responseSlider);
    responseSlider.setRange (0, 2, 1); // Makes the slider discrete

    frequencyLabel.setText ("Frequency", dontSendNotification);
    addAndMakeVisible (frequencyLabel);
    addAndMakeVisible (frequencySlider);
    frequencySlider.setSliderStyle(Slider::Rotary);
    frequencyAttachment = new SliderAttachment (valueTreeState, "frequency", frequencySlider);
    frequencySlider.setSkewFactor (0.5); // skews the frequency cut off slider so that the low frequencies have more space
    frequencySlider.setValue (2000);


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 600);
}

DelayAudioProcessorEditor::~DelayAudioProcessorEditor()
{
}

//==============================================================================
void DelayAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void DelayAudioProcessorEditor::resized ()
{
    //GUI layout settings
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

        Rectangle<int> responseRect = r.removeFromTop (paramControlHeight);
        responseLabel.setBounds (responseRect.removeFromLeft (paramLabelWidth));
        responseSlider.setBounds (responseRect);

        Rectangle<int> frequencyRect = r.removeFromTop (paramControlHeight);
        frequencyLabel.setBounds (frequencyRect.removeFromLeft (paramLabelWidth));
        frequencySlider.setBounds (frequencyRect);
    }
}



