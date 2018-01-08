/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
AudioPluginTestAudioProcessorEditor::AudioPluginTestAudioProcessorEditor (AudioPluginTestAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p),
    processor (p),
    valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    gainLabel.setText ("Gain", dontSendNotification);
    addAndMakeVisible (gainLabel);

    addAndMakeVisible (gainSlider);
    gainAttachment = new SliderAttachment (valueTreeState, "gain", gainSlider);

    setSize (paramSliderWidth + paramLabelWidth, paramControlHeight * 2);
}

AudioPluginTestAudioProcessorEditor::~AudioPluginTestAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginTestAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void AudioPluginTestAudioProcessorEditor::resized()
{
    Rectangle<int> r = getLocalBounds ();

    {
        Rectangle<int> gainRect = r.removeFromTop (paramControlHeight);
        gainLabel.setBounds (gainRect.removeFromLeft (paramLabelWidth));
        gainSlider.setBounds (gainRect);
    }
}



