#include "errorHandling.h"

typedef struct
{
    const int exitCode;
    const char* message;
    const char extra;
} err;

const err errorList[] =
{
    {err_success, "Success!\n", '\0'},
    {err_wav_not_mono, "The input file is not mono. It has %d channel(s)\n", '\0'},
    {err_wrong_number_of_inputs, "Expected 3 arguments, but the number of arguments given was %d.\n", 'd'},
    {err_wav_not_open, "Unable to open file at '%s'. \n", 's'},
    {err_option_not_recognised, "The argument flag %c, was not recognised.\n", 'c'},
    {err_buffer_too_big, "The maximum buffer size is %d.\n", 'd'},
    {err_buffer_not_pow_two, "The buffer must be a power of two.\n", '\0'},
    {err_argument_not_number, "'%s' not recognised, expecting a number.\n", 's'}
    
};

void errorHandler (int code, ...)
{
    if (code >= err_num_errors)
    {
        printf ("Error code not recognised");
        exit (code);
    }

    int index = code == 0 ? code : code - dsp_err_num_errors;
    
    const char* message = errorList[index].message;

    va_list args;
    va_start (args, code);

    if (errorList[index].extra == '\0')
    {
        printf ("%s", message);
    }
    else if (errorList[index].extra == 'd')
    {
        int d = va_arg (args, int);
        printf (message, d);
    }
    else if (errorList[index].extra == 'c')
    {
        char c = va_arg (args, int);
        printf (message, c);
    }
    else if (errorList[index].extra == 's')
    {
        char* string = va_arg (args, char*);
        printf (message, string);
    }

    exit (code);
}

