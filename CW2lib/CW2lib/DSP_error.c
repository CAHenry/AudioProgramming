#include "DSP_error.h"

typedef struct
{
    const int exitCode;
    const char* message;
    const char extra;
    bool quit;
} DSP_err;

const DSP_err DSP_errorList[] =
{
    {dsp_err_success, "Success!\n", '\0', true},
    {dsp_err_memory_not_allocated, "Failed to allocate memory.\n", '\0', true},
    {dsp_err_wav_not_open, "Unable to open the file at the following path: %s\n", 's', true},
    {dsp_war_buffer_too_big, "The maximum buffer size is 2048, the value has been used rather than the user defined size of %d", 'd', false},
    {dsp_war_buffer_not_pow_two, "The buffer size must be a power of two, %d does not comply. Buffer size has reverted to the default of 2048", 'd', false},
    {dsp_err_write_error, "An error has occurred when writing to a file", '\0', true}
};

const int DSP_numOfErrors = sizeof (DSP_errorList) / sizeof (DSP_err);

void DSP_errorHandler (unsigned int code, ...)
{
    if (code >= DSP_numOfErrors)
    {
        printf ("Error code not recognised");
        exit (code);
    }

    char* message = DSP_errorList[code].message;

    va_list args;
    va_start (args, code);

    if (DSP_errorList[code].extra == '\0')
    {
        printf (message);
    }
    else if (DSP_errorList[code].extra == 'd')
    {
        int d = va_arg (args, int);
        printf (message, d);
    }
    else if (DSP_errorList[code].extra == 's')
    {
        char* string = va_arg (args, char*);
        printf (message, string);
    }

    if (DSP_errorList[code].quit)
    {
        exit (code);
    }
}


void DSP_allocCheck (void* ptr)
{
    if (ptr == NULL)
    {
        DSP_errorHandler (dsp_err_memory_not_allocated);
    }
}

bool DSP_isPowerOfTwo (int val)
{
    while ((val % 2) == 0 && val > 1)
    {
        val /= 2;
    }
    return val == 1;
}