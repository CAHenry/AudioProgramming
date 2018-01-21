#include "miscErrorChecks.h"

typedef struct
{
    const int exitCode;
    const char* message;
    const char extra;
    const bool quit;
} err;

const err errorList[] =
{
    {err_success, "Success!\n", '\0', true},
    {err_wav_not_mono, "The input file is not mono. It has %d channel(s)\n", '\0', true},
    {err_wrong_number_of_inputs, "Expected 3 arguments, but the number of arguments given was %d.\n", 'd', true},
    {err_option_not_recognised, "The argument %c, was not recognised.\n", 'c', true}
};
const int numOfErrors = sizeof (errorList) / sizeof (err);

void errorHandler (int code, ...)
{
    if (code >= numOfErrors)
    {
        printf ("Error code not recognised");
        exit (code);
    }

    const char* message = errorList[code].message;

    va_list args;
    va_start (args, code);

    if (errorList[code].extra == '\0')
    {
        printf ("%s", message);
    }
    else if (errorList[code].extra == 'd')
    {
        int d = va_arg (args, int);
        printf (message, d);
    }
    else if (errorList[code].extra == 'c')
    {
        char c = va_arg (args, int);
        printf (message, c);
    }

    if (code != err_success)
    {
        printf ("Run the program with no arguments for more information.\n");
    }

    exit (code);
}


void wavMonoCheck (DSP_file* file)
{
    int numChannels = DSP_getNumChannels (file);

    if (numChannels != 1)
    {
        errorHandler (err_wav_not_mono, numChannels);
    }
}
