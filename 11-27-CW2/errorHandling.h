#ifndef ERRORHANDLING_H
#define ERRORHANDLING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include "dsp_error.h"

enum config_exit_code
{
    err_success,
    err_wav_not_mono = dsp_err_num_errors,
    err_wrong_number_of_inputs,
    err_wav_not_open,
    err_option_not_recognised,
    err_buffer_too_big,
    err_buffer_not_pow_two,
    err_argument_not_number,
    err_num_errors
};

void errorHandler (int code, ...);

#endif
