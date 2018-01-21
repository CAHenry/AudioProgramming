/*
  ==============================================================================

    CustomSlider.h
    Created: 20 Jan 2018 5:54:31pm
    Author:  Craig

  ==============================================================================
*/
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class TextParameterSlider : public Slider
    //==============================================================================
    /**
    Custom Slider class. Inherets from the juce::Slider class. Provides the functionality
    to set the text to value functions from the Slider class. This allows the slider
    to set the text of the label to be custom values. If the functions are not overridden
    then the class defaults to the juce:Slider functions.

    see juce::Slider
    */
{
public:
    /** Set the function to override the slider getTextFromValue function */
    void setGetTextFromValueFunction (std::function <String (double value)> getTextFromValueFunc);

    /** Set the function to override the slider getValueFromText function */
    void setGetValueFromTextFunction (std::function <double (String text)> getValueFromTextFunc);
    
    //==============================================================================
    /** Called by the slider's attached label to set its text from the slider value */
    String getTextFromValue (double value) override;

    /** Called by the slider to set its value from the text in the attached label   */
    double getValueFromText (const String &text) override;

private:
    // Function pointers to replace the getTextFromValue and getValueFromText functions
    std::function <String (double value)> getTextFromValueFunction;
    std::function <double (String text)> getValueFromTextFunction;
};
