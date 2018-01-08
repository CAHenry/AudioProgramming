/**********************************************************************
 * FILE NAME: dsp_error.c                                             *
 *                                                                    *
 * PURPOSE: error handling for dsp library functions                  *
 *                                                                    *
 * EXTERNAL VARIABLES: none                                           *
 *                                                                    *
 * REQUIREMENTS: dsp_error.h, stdlib.h, stdio.h and stdarg.h          *
 *********************************************************************/

#include "dsp_error.h"

/**************************************************************************************************/

/*
 * contains all information relevant to an error:
 *     - Exit code <exitCode>
 *     - The error message printed to terminal <message>
 *     - A printf style identifier for extra information required by the message <extraInfoType>
 */
typedef struct
{
    const int exitCode;
    const char* message;
    const char extraInfoType;
} dsp_err;

/**************************************************************************************************/

/*
 * List of errors included in the dsp_error enum in dsp_error.h. Must be kept in sync with the
 * enum and any additional info must be stated in the header file documentation.
 */
const dsp_err dsp_errorList[] =
{
    {dsp_err_memory_not_allocated, "Failed to allocate memory.\n", '\0'},
    {dsp_err_filter_response_not_recognised, "Filter flag '%d' not recognised.\n", 'd'},
    {dsp_err_window_function_not_recognised, "Window flag '%d' not recognised.\n", 'd'},
    {dsp_err_hpf_not_even, "High pass filters must have an even order.\n", '\0'}
};

/**************************************************************************************************/

/*
 * Takes an error code and any additional information (if applicable).
 * Prints the error message along with any extra info and exits with the enumerated exit code.
 * NOTE: calling this function always exits the program.
 */
void dsp_errorHandler (dsp_error_code code, ...)
{
    if (code >= dsp_err_num_errors)
    {
        printf ("Error code not recognised.\n");
        exit (code);
    }
    
    /* Create a list of arguments <args> starting after <code>.
     * va_arg can then be used to access these arguments. */
    va_list args;
    va_start (args, code);
    
    /* Check <extraInfoType> and then print <message> with
     * any extra info provided as an argument <va_arg> */
    if (dsp_errorList[code].extraInfoType == '\0')
    {
        printf ("%s", dsp_errorList[code].message);
    }
    else if (dsp_errorList[code].extraInfoType == 'd')
    {
        int d = va_arg (args, int);
        printf (dsp_errorList[code].message, d);
    }
    else if (dsp_errorList[code].extraInfoType == 'c')
    {
        char c = va_arg (args, int);
        printf (dsp_errorList[code].message, c);
    }
    else if (dsp_errorList[code].extraInfoType == 's')
    {
        char* string = va_arg (args, char*);
        printf (dsp_errorList[code].message, string);
    }
    exit (code);
}
