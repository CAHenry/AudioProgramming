#ifndef DSP_ERROR_H
#define DSP_ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

static const int g_codeOffset = 100;

enum dsp_config_exit_code
{
    dsp_err_memory_not_allocated = g_codeOffset,
    dsp_err_response_out_of_bounds,
    dsp_err_window_out_of_bounds
};

void dsp_errorHandler (unsigned int code, ...);



#endif
