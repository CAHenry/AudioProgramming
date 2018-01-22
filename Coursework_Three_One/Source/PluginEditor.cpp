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
    getLookAndFeel ().setColour (Slider::thumbColourId, juce::Colour::fromRGB (126, 179, 142));
    getLookAndFeel ().setColour (Slider::textBoxOutlineColourId, getLookAndFeel ().findColour(ResizableWindow::backgroundColourId));

      // Initialising GUI components
    titleLabel.setText ("Delay Plugin", dontSendNotification);
    titleLabel.setJustificationType (Justification::centred);
    addAndMakeVisible (titleLabel);
    titleLabel.setFont (Font (20.0f, Font::bold));


    mixLabel.setText ("Mix", dontSendNotification);
    addAndMakeVisible (mixLabel);
    addAndMakeVisible (mixSlider);
    mixSlider.setSliderStyle (Slider::Rotary);
    mixSlider.setTextValueSuffix ("%");
    mixSlider.setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    mixLabel.attachToComponent (&mixSlider, false);
    mixLabel.setJustificationType (Justification::centred);
    mixAttachment = new SliderAttachment (valueTreeState, "mix", mixSlider);
    mixSlider.setRange (0.00, 100.0, 0.1);

    feedbackLabel.setText ("Feedback", dontSendNotification);
    addAndMakeVisible (feedbackLabel);
    addAndMakeVisible (feedbackSlider);
    feedbackSlider.setSliderStyle (Slider::Rotary);
    feedbackSlider.setTextValueSuffix ("%");
    feedbackSlider.setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    feedbackLabel.attachToComponent (&feedbackSlider, false);
    feedbackLabel.setJustificationType (Justification::centred);
    feedbackAttachment = new SliderAttachment (valueTreeState, "feedback", feedbackSlider);
    feedbackSlider.setRange (0.00, 100.0, 0.1);

    timeLabel.setText ("Time", dontSendNotification);
    addAndMakeVisible (timeLabel);
    addAndMakeVisible (timeSlider);
    timeSlider.setSliderStyle (Slider::Rotary);
    timeSlider.setTextValueSuffix ("s");
    timeSlider.setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    timeLabel.attachToComponent (&timeSlider, false);
    timeLabel.setJustificationType (Justification::centred);
    timeAttachment = new SliderAttachment (valueTreeState, "time", timeSlider);
    timeSlider.setRange (0.01, 1.0, 0.01);

    responseLabel.setText ("Response", dontSendNotification);
    addAndMakeVisible (responseLabel);
    addAndMakeVisible (responseSlider);
    responseSlider.setGetTextFromValueFunction (Filter::responseToText); //replace text and value functions
    responseSlider.setGetValueFromTextFunction (Filter::textToResponse);
    responseSlider.setSliderStyle (Slider::Rotary);
    responseSlider.setTextBoxStyle (Slider::TextBoxBelow, true, 80, 20);
    responseLabel.attachToComponent (&responseSlider, false);
    responseLabel.setJustificationType (Justification::centred);
    responseAttachment = new SliderAttachment (valueTreeState, "response", responseSlider);
    responseSlider.setRange (0, 2, 1); // Makes the slider discrete

    frequencyLabel.setText ("Frequency", dontSendNotification);
    addAndMakeVisible (frequencyLabel);
    addAndMakeVisible (frequencySlider);
    frequencySlider.setSliderStyle(Slider::Rotary);
    frequencySlider.setTextValueSuffix ("Hz");
    frequencySlider.setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    frequencyLabel.attachToComponent (&frequencySlider, false);
    frequencyLabel.setJustificationType (Justification::centred);
    frequencyAttachment = new SliderAttachment (valueTreeState, "frequency", frequencySlider);
    frequencySlider.setRange (20, 20000, 1);
    frequencySlider.setSkewFactor (0.5); // skews the frequency cut off slider so that the low frequencies have more space


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (350, 300);
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

    Rectangle<int> area = getLocalBounds ();
    area.removeFromBottom (area.getHeight () * 0.05);
    Rectangle<int> title = area.removeFromTop (area.getHeight () * 0.2);
    Rectangle<int> delayBox = area.removeFromTop (area.getHeight () * 0.5);
    
    titleLabel.setBounds (title);

    delayBox.removeFromTop (paramLabelHeight);
    Rectangle<int> mixRect = delayBox.removeFromLeft (delayBox.getWidth()/3.0);
    mixSlider.setBounds (mixRect);

    Rectangle<int> feedbackRect = delayBox.removeFromLeft (delayBox.getWidth () / 2.0);
    feedbackSlider.setBounds (feedbackRect);

    Rectangle<int> timeRect = delayBox;
    timeSlider.setBounds (timeRect);
    
    Rectangle<int> filterBox(mixRect.getCentreX(), area.getY(), timeRect.getCentreX () - mixRect.getCentreX () , area.getHeight());
    filterBox.removeFromTop (paramLabelHeight);
    Rectangle<int> responseRect = filterBox.removeFromLeft (filterBox.getWidth () / 2.0);
    responseSlider.setBounds (responseRect);

    Rectangle<int> frequencyRect = filterBox;
    frequencySlider.setBounds (frequencyRect);
    
}



