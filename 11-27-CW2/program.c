#include "program.h"

void checkNumber (char* number)
{
    int i = 0;
    
    //checking for negative numbers
    if (number[0] == '-')
    {
        i = 1;
    }
    for (; number[i] != 0; i++)
    {
        //if (number[i] > '9' || number[i] < '0')
        if (!isdigit(number[i]))
        {
            errorHandler(err_argument_not_number, number);
        }
    }
}

bool isValPowerOfTwo (int val)
{
    while ((val % 2) == 0 && val > 1)
    {
        val /= 2;
    }
    return val == 1;
}

void checkBufferSizeValid (int bufferSize)
{
    if (bufferSize > g_maxBufferSize)
    {
        errorHandler(err_buffer_too_big, g_maxBufferSize);
    }
    else if ( ! isValPowerOfTwo(bufferSize))
    {
        errorHandler(err_buffer_not_pow_two);
    }
}

int handleOptionalArgs (const int argc, char** argv, windowFlag* window, filterFlag* response, int* bufferSize)
{
    int opt;
    while ((opt = getopt (argc, argv, "w:f:b:")) != -1)
    {
        switch (opt)
        {
            case 'w':
                checkNumber (optarg);
                *window = atoi(optarg);
                break;
            case 'f':
                checkNumber (optarg);
                *response = atoi(optarg);
                break;
            case 'b':
                checkNumber (optarg);
                *bufferSize = atoi(optarg);
                checkBufferSizeValid(*bufferSize);
                break;
            case '?':
                errorHandler (err_option_not_recognised, optopt);
                break;
        }
    }
    return optind;
}

void instructions(void);
void borderBS(void);

void handleCompulsoryArgs (int numOfOptionalArgs, const int argc, char** argv, char** inputPath, char** outputPath, int* cutoff)
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
        *inputPath = argv[numOfOptionalArgs];
        *outputPath = argv[numOfOptionalArgs + 1];
        checkNumber (argv[numOfOptionalArgs + 2]);
        *cutoff = atoi (argv[numOfOptionalArgs + 2]);
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


