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
    {dsp_err_memory_not_allocated, "Failed to allocate memory.\n", '\0', true},
    {dsp_err_wav_not_open, "Unable to open the file at the following path: %s\n", 's', true},
    {dsp_war_buffer_too_big, "The maximum buffer size of %d has been exceeded, the maximum value has been used.\n", 'd', false},
    {dsp_war_buffer_not_pow_two, "The buffer size must be a power of two. Buffer size has reverted to the default of %d.\n", 'd', false},
    {dsp_err_write_error, "An error has occurred when writing to a file", '\0', true}
};

const int DSP_numOfErrors = sizeof (DSP_errorList) / sizeof (DSP_err);

void DSP_errorHandler (unsigned int code, ...)
{
    if (code >= DSP_numOfErrors + g_codeOffset)
    {
        printf ("Error code not recognised");
        exit (code);
    }
    
    int index = code - g_codeOffset;

    const char* message = DSP_errorList[index].message;

    va_list args;
    va_start (args, code);

    if (DSP_errorList[index].extra == '\0')
    {
        printf ("%s", message);
    }
    else if (DSP_errorList[index].extra == 'd')
    {
        int d = va_arg (args, int);
        printf (message, d);
    }
    else if (DSP_errorList[index].extra == 's')
    {
        char* string = va_arg (args, char*);
        printf (message, string);
    }

    if (DSP_errorList[index].quit)
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
