#include "program.h"


int optionalArgs (const int argc, char** argv, windowFlag* window, filterFlag* response, int* bufferSize)
{
    int opt;
    while ((opt = getopt (argc, argv, "w:f:b:")) != -1)
    {
        switch (opt)
        {
            case 'w':
                *window = atoi(optarg); // TODO: error checking within bounds
                break;
            case 'f':
                *response = atoi(optarg); // TODO: error checking within bounds
                break;
            case 'b':
                *bufferSize = atoi(optarg); // TODO: error checking within bounds
                break;
            case '?':
                errorHandler (err_option_not_recognised, optopt);
                break;
        }
    }
    return optind;
}

void instructions();
void borderBS();

void compulsoryArgs (int numOfOptionalArgs, const int argc, char** argv, char** inputPath, char** outputPath, int* cutoff)
{
    int numOfCompulsoryArgs = argc - numOfOptionalArgs;
    
    if (numOfCompulsoryArgs == 0)
    {
        instructions ();
        exit (0);
    }
    else if (numOfCompulsoryArgs != 3)
    {
        errorHandler (err_wrong_number_of_inputs, numOfCompulsoryArgs);
    }
    else
    {
        *inputPath = argv[numOfOptionalArgs]; // TODO: error checking for isString
        *outputPath = argv[numOfOptionalArgs + 1]; // TODO: error checking for isString
        *cutoff = atoi (argv[numOfOptionalArgs + 2]); // TODO: error checking for isInteger
    }
}

void borderBS ()
{

}

void instructions ()
{
    borderBS ();
}


void openWavs (SNDFILE** input, char* inputPath, SNDFILE** output, char* outputPath, SF_INFO* fileInfo)
{
    if ( ! (*input = sf_open (inputPath, SFM_READ, fileInfo)))
    {
        errorHandler (err_wav_not_open, inputPath);
    }
    
    if ( ! (*output = sf_open (outputPath, SFM_WRITE, fileInfo)))
    {
        errorHandler (err_wav_not_open, outputPath);
    }
    
    if (fileInfo->channels != 1)
    {
        errorHandler (err_wav_not_mono, fileInfo->channels);
    }
}

void closeWavs (SNDFILE* input, SNDFILE* output)
{
    sf_close(input);
    sf_close(output);
}

bool isValPowerOfTwo (int val)
{
    while ((val % 2) == 0 && val > 1)
    {
        val /= 2;
    }
    return val == 1;
}
