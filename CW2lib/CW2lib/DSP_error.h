#ifndef DSP_ERROR_H
#define DSP_ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

const enum dsp_config_exit_code
{
    dsp_err_success = 0,
    dsp_err_memory_not_allocated,
    dsp_err_wav_not_open,
    dsp_war_buffer_too_big,
    dsp_war_buffer_not_pow_two,
    dsp_err_write_error,
    dsp_err_open_value
};

void DSP_errorHandler (unsigned int code, ...);

void DSP_allocCheck (void* ptr);

bool DSP_isPowerOfTwo (int val);

#endif