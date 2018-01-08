#ifndef DSP_ERROR_H
#define DSP_ERROR_H

#include <stdio.h> // required for printf();
#include <stdlib.h> // required for exit();
#include <stdarg.h> // required for variadic function dsp_errorHandler, see varargs.

/*
 * Error code enumerator for the dsp functions and structs. 
 */
typedef enum
{
    dsp_err_memory_not_allocated,           // Some memory was unable to be allocated
    dsp_err_filter_response_not_recognised, // The filter response requested was not recognised.
        /* extra info: Filter flag input */
    dsp_err_window_function_not_recognised, // The window function requested was not recognised.
        /* extra info: Window flag input */
    dsp_err_hpf_not_even,                   // High pass filters require an even filter order
    dsp_err_num_errors                      // NOT AN ERROR: Should be used to avoid exit code clashes.
} dsp_error_code;

/*
 * Print the error message for error code <code> and exit with the same error code. Some error code messages
 * provide additional information about the error. This infomation is passed as a second argument to this function.
 * See the documentation at the dsp_error_code enumerator to find out which error codes require what information.
 */
void dsp_errorHandler (dsp_error_code code, ...);

#endif
