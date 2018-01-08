/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
AudioPluginTestAudioProcessorEditor::AudioPluginTestAudioProcessorEditor (AudioPluginTestAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    const OwnedArray<AudioProcessorParameter>& params = processor.getParameters ();

    for (int i = 0; i < params.size (); ++i)
    {
        if (const AudioParameterFloat* param = dynamic_cast<AudioParameterFloat*> (params[i]))
        {
            Slider* aSlider;

            paramSliders.add (aSlider = new Slider (param->name));
            aSlider->setRange (param->range.start, param->range.end);
            aSlider->setSliderStyle (Slider::LinearHorizontal);
            aSlider->setValue (*param);

            aSlider->addListener (this);
            addAndMakeVisible (aSlider);

            Label* aLabel;
            paramLabels.add (aLabel = new Label (param->name, param->name));
            addAndMakeVisible (aLabel);
        }
    }
    setSize (kParamSliderWidth + kParamLabelWidth, jmax (1, kParamControlHeight * paramSliders.size ()));

    startTimer (100);
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
    g.drawFittedText ("Gain Slider", 0, 0, getWidth(), 30, Justification::centred, 1);
}

void AudioPluginTestAudioProcessorEditor::resized()
{
    Rectangle<int> r = getLocalBounds ();

    for (int i = 0; i < paramSliders.size (); ++i)
    {
        Rectangle<int> paramBounds = r.removeFromTop (kParamControlHeight);
        Rectangle<int> labelBounds = paramBounds.removeFromLeft (kParamLabelWidth);

        paramLabels[i]->setBounds (labelBounds);
        paramSliders[i]->setBounds (paramBounds);
    }
}

void AudioPluginTestAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    if (AudioParameterFloat* param = getParameterForSlider (slider))
        *param = (float) slider->getValue ();
}

void AudioPluginTestAudioProcessorEditor::sliderDragStarted (Slider* slider)
{
    if (AudioParameterFloat* param = getParameterForSlider (slider))
        param->beginChangeGesture ();
}

void AudioPluginTestAudioProcessorEditor::sliderDragEnded (Slider* slider)
{
    if (AudioParameterFloat* param = getParameterForSlider (slider))
        param->endChangeGesture ();
}

AudioParameterFloat* AudioPluginTestAudioProcessorEditor::getParameterForSlider (Slider* slider)
{
    const OwnedArray<AudioProcessorParameter>& params = getAudioProcessor ()->getParameters ();
    return dynamic_cast<AudioParameterFloat*> (params[paramSliders.indexOf (slider)]);
}

void AudioPluginTestAudioProcessorEditor::timerCallback ()
{
    const OwnedArray<AudioProcessorParameter>& params = getAudioProcessor ()->getParameters ();
    for (int i = 0; i < params.size (); ++i)
    {
        if (const AudioParameterFloat* param = dynamic_cast<AudioParameterFloat*> (params[i]))
        {
            if (i < paramSliders.size ())
                paramSliders[i]->setValue (*param);
        }
    }
}
