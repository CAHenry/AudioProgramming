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

    syncTimeLabel.setText ("Sync Time", dontSendNotification);
    addAndMakeVisible (syncTimeLabel);
    addAndMakeVisible (syncTimeSlider);
    syncTimeSlider.setGetTextFromValueFunction (Delay::syncToText);
    syncTimeSlider.setGetValueFromTextFunction (Delay::textToSync);
    syncTimeLabel.setVisible (false);
    syncTimeSlider.setVisible (false);
    syncTimeSlider.setSliderStyle (Slider::Rotary);
    syncTimeAttachment = new SliderAttachment (valueTreeState, "syncTime", syncTimeSlider);
    syncTimeSlider.setRange (0, 6, 1);

    syncLabel.setText ("Sync", dontSendNotification);
    addAndMakeVisible (syncLabel);
    addAndMakeVisible (syncButton);
    syncAttachment = new ButtonAttachment (valueTreeState, "sync", syncButton);

    responseLabel.setText ("Filter Response", dontSendNotification);
    addAndMakeVisible (responseLabel);
    addAndMakeVisible (responseSlider);
    responseSlider.setGetTextFromValueFunction (Filter::responseToText);
    responseSlider.setGetValueFromTextFunction (Filter::textToResponse);
    responseSlider.setSliderStyle (Slider::Rotary);
    responseAttachment = new SliderAttachment (valueTreeState, "response", responseSlider);
    responseSlider.setRange (0, 2, 1);

    frequencyLabel.setText ("Frequency", dontSendNotification);
    addAndMakeVisible (frequencyLabel);
    addAndMakeVisible (frequencySlider);
    frequencySlider.setSliderStyle(Slider::Rotary);
    frequencyAttachment = new SliderAttachment (valueTreeState, "frequency", frequencySlider);
    frequencySlider.setSkewFactor (0.5);


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 600);
    startTimer (100);
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

void DelayAudioProcessorEditor::resized ()
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
        timeLabel.setBounds (timeRect);
        syncTimeLabel.setBounds (timeRect.removeFromLeft (paramLabelWidth));
        timeSlider.setBounds (timeRect);
        syncTimeSlider.setBounds (timeRect);

        Rectangle<int> syncRect = r.removeFromTop (paramControlHeight);
        syncLabel.setBounds (syncRect.removeFromLeft (paramLabelWidth));
        syncButton.setBounds (syncRect);

        Rectangle<int> responseRect = r.removeFromTop (paramControlHeight);
        responseLabel.setBounds (responseRect.removeFromLeft (paramLabelWidth));
        responseSlider.setBounds (responseRect);

        Rectangle<int> frequencyRect = r.removeFromTop (paramControlHeight);
        frequencyLabel.setBounds (frequencyRect.removeFromLeft (paramLabelWidth));
        frequencySlider.setBounds (frequencyRect);
    }
}

void DelayAudioProcessorEditor::timerCallback ()
{
    static bool sync = true;
    bool syncStatus = processor.delay.getSync ();

    if (syncStatus != sync)
    {
        if (syncStatus)
        {
            timeLabel.setVisible (false);
            timeSlider.setVisible (false);
            syncTimeLabel.setVisible (true);
            syncTimeSlider.setVisible (true);
        }
        else
        {
            timeLabel.setVisible (true);
            timeSlider.setVisible (true);
            syncTimeLabel.setVisible (false);
            syncTimeSlider.setVisible (false);
        }
        
        sync = syncStatus;
    }
}


