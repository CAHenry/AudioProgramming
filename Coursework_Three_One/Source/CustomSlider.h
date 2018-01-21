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
{
public:
    void setGetTextFromValueFunction (std::function <String (double value)> getTextFromValueFunc);
    void setGetValueFromTextFunction (std::function <double (String text)> getValueFromTextFunc);

    String getTextFromValue (double value) override;
    double getValueFromText (const String &text) override;

private:
    std::function <String (double value)> getTextFromValueFunction;
    std::function <double (String text)> getValueFromTextFunction;
};
