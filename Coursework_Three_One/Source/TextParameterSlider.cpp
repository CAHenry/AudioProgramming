/*
  ==============================================================================

    CustomSlider.cpp
    Created: 20 Jan 2018 5:54:31pm
    Author:  Craig

  ==============================================================================
*/

#include "TextParameterSlider.h"

void TextParameterSlider::setGetTextFromValueFunction (std::function<String (double value)> getTextFromValueFunc)
{
    getTextFromValueFunction = getTextFromValueFunc;
}

void TextParameterSlider::setGetValueFromTextFunction (std::function<double (String text)> getValueFromTextFunc)
{
    getValueFromTextFunction = getValueFromTextFunc;
}

String TextParameterSlider::getTextFromValue (double value)
{
    if (getTextFromValueFunction != nullptr) 
        return getTextFromValueFunction (value);
    else
        return Slider::getTextFromValue (value); // if a replacement function has not been set revert to default function
}

double TextParameterSlider::getValueFromText (const String &text)
{
    if (getValueFromTextFunction != nullptr)
        return getValueFromTextFunction (text);
    else
        return Slider::getValueFromText (text); // if a replacement function has not been set revert to default function
}
