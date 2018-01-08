#include "dsp_error.h"

typedef struct
{
    const int exitCode;
    const char* message;
    const char extra;
    bool quit;
} dsp_err;

// TODO: ensure these are not over 80 chars
const dsp_err dsp_errorList[] =
{
    {dsp_err_memory_not_allocated, "Failed to allocate memory.\n", '\0', true},
};

const int dsp_numOfErrors = sizeof (dsp_errorList) / sizeof (dsp_err);

void dsp_errorHandler (unsigned int code, ...)
{
    if (code >= dsp_numOfErrors + g_codeOffset)
    {
        printf ("Error code not recognised");
        exit (code);
    }
    
    int index = code - g_codeOffset;

    const char* message = dsp_errorList[index].message;

    va_list args;
    va_start (args, code);

    if (dsp_errorList[index].extra == '\0')
    {
        printf ("%s", message);
    }
    else if (dsp_errorList[index].extra == 'd')
    {
        int d = va_arg (args, int);
        printf (message, d);
    }
    else if (dsp_errorList[index].extra == 's')
    {
        char* string = va_arg (args, char*);
        printf (message, string);
    }

    if (dsp_errorList[index].quit)
    {
        exit (code);
    }
}
