// error handling 
// argument -help prints instructions and then return
// programme run with no arguments it must wait until a valid input is enter, it continues to wait until a negative midi value is sent.
// any input that isn't time (in ms) and a midi note value should print "User input not in a recognised format" and then exit
// any midi date greater than 127 prints "The MIDI 'noteon' message contains data out of bounds." and then exit
// any negative or not increasing time values should print "The time values need to be non-negative and increasing in value." and then exit
// any errors should cause the programme to exit before any samples are printed, i.e negative midi triggers printing to begin.
// the 100th note is always treated as -1 regardless of input, thereby limiting the input to 100
// sampling frequency is 48kHz
// samples are printed with 6 decimal places
// the first sample should always be 0.000000 from 0 time?
// no phase discontinuities
// no header files, single .cfile
// must compile correctly using gcc with no errors or warnings, using the command: gcc -Wall <filename>
// file should be cw1_CraigHenry.c
// test on Mac in self-service lab under clock, serial number DGKN9078F8J9
// marked on meeting the spec above and also:
// compilation: no errors or warnings
// efficiency and elegance of code
// documentation of source code
// structure of the code
// the quality of the user interface (help text)

/*
undefined behaviour in the spec:
Input via argv but no midi -1? Does it pretend the last note is -1 or does it wait for further instruction
Should the first sample always be from 0 in time as when as 0 in amplitude, regardless of starting time in ms
Does sinewave need to be accurate to 6 decimel places or correct audibly i.e. will my wave table be okay
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>


inline double mToHz (int midi)
{
    if (midi < 0)
    {
        return midi;
    }

    return pow (2.0, (midi - 69) / 12);
}   

inline double interpolate (const double* phase, double* table)
{
    int low = (int) *phase;
    int high = low + 1;

    double fPart = *phase - low;

    return (table[low] * (1 - fPart)) + (table[high] * fPart);
}

void help ()
{
    //fancy *** borders with centrally justified text etc for default terminal size
    printf ("help instructions");
    exit (0);
}

bool readingInput (char* input[], int* index, int* times, int* frequencies)
{
    int inputTime = atoi (input[*index * 2]);
    int inputMIDI = atoi (input[*index * 2 + 1]);

    if (inputMIDI > 127)
    {
        printf ("The MIDI 'noteon' message contains data out of bounds.");
        exit (0);
    }

    if (inputTime < 0 || (index > 0 && inputTime <= times[*index - 1]))
    {
        printf ("The time values need to be non-negative and increasing in value.");
        exit (0);
    }

    times[*index] = inputTime;
    frequencies[*index] = index < 100 ? mToHz (inputMIDI) : -1;

    return (frequencies[*index] >= 0);
}

void fillWavetable (const int* tableSize, double* sineTable)
{
    const float PI = 3.14159265359;

    for (float i = 0.0; i < *tableSize; i++)
    {
        sineTable[(int) i] = sin ((i / *tableSize) * 2.0 * PI);
    }
}

void printSamples (const int* tableSize, double* sineTable)
{
    double f = 2000;
    const double fs = 48000;

    double angleDelta = (f / fs) * (double) *tableSize;
    double phase = 0;

    int lengthMilliseconds = 4;
    int lengthSamples = fs * ((float) lengthMilliseconds / 1000.0);
    int start = lengthSamples;
    printf ("%d\n\n", lengthSamples);

    while (lengthSamples-- > 0)
    {
        double sample = interpolate (&phase, sineTable);
        printf ("%f\n", sample);
        phase = fmod ((phase + angleDelta), (double) *tableSize);
    }
}

int main (const int argc, char* argv[])
{
    bool inputByScan = false;
    int inputSize;

    if (argv[1] == "-help")
    {
        help ();
    }
    else if (argc == 1)
    {
        inputByScan = true;
        inputSize = 100;
    }
    else
    {
        inputSize = argc - 1;
    }


    char* input = inputByScan ? (char*) malloc ((inputSize * sizeof(char))): argv + 1;
    int* times = (int*) malloc ((inputSize / 2) * sizeof (int));
    double* frequencies = (double*) malloc ((inputSize / 2) * sizeof (double));


    int index = 0;

    do
    {
        if (inputByScan)
        {
            scanf ("%s %s", input[index * 2], input[index * 2 + 1]);
        }

        if (index > inputSize / 2)
        {
            inputByScan = true;
        }
    }
    while (readingInput (input, index++, times, frequencies));
    

    const int tableSize = 1024;
    double* sineTable = (double*) malloc (tableSize * sizeof (double));

    fillWavetable (&tableSize, sineTable);

    printSamples (&tableSize, sineTable);

    return 0;
}