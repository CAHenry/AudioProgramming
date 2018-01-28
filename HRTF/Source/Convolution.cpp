/*
  ==============================================================================

    Convolution.cpp
    Created: 27 Jan 2018 11:25:16am
    Author:  Craig

  ==============================================================================
*/

#include "Convolution.h"

Convolution::Convolution ()
{
    FileChooser chooser ("Select a Wave file shorter than 2 seconds to play...",
        File::nonexistent,
        "*.wav");
    chooser.browseForFileToOpen ();
    impulseFile = chooser.getResult ();
}

Convolution::Convolution (String impulseWav, AudioBuffer<float> &impulse, AudioBuffer<float> &convLine)  
{
    FileChooser chooser ("Select a Wave file shorter than 2 seconds to play...",
        File::nonexistent,
        "*.wav");
    chooser.browseForFileToOpen (); 
    impulseFile = chooser.getResult ();
}


void Convolution::prepare (AudioBuffer<float>& impulse, AudioBuffer<float>& convLine)
{
    formatManager.registerBasicFormats ();

    AudioFormatReader* reader (formatManager.createReaderFor (impulseFile));

    if (reader != nullptr)
    {
        size = reader->lengthInSamples;
        const double duration = size / reader->sampleRate;
        convLine.setSize (reader->numChannels, size);
        impulse.setSize (reader->numChannels, size);
        reader->read (&impulse, 0, size, 0, true, true);
    }
}

void Convolution::convolve (float* sample, float* convLine, float* impulse)
{
    if (sample[0] != 0)
    {
        int test = 2;
    }

    convLine[index % size] = sample[0];
    sample[0] = 0;
    size = 1024;
    for (int i = 0; i < size; i++)
    {
        sample[0] += convLine[(index + i + 1) % size] * impulse[size - 1 - i];
        //DBG (String (convLine[(index + i + 1) % size]));
        //DBG (String (impulse[size - 1 - i]));
    }


    index = (index + 1) % size;
}
