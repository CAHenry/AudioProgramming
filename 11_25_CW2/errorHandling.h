#ifndef ERRORHANDLING_H
#define ERRORHANDLING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

enum config_exit_code
{
    err_success,
    err_wav_not_mono,
    err_wrong_number_of_inputs,
    err_option_not_recognised,
    err_wav_not_open,
    war_buffer_too_big,
    war_buffer_not_pow_two,
};

void errorHandler (int code, ...);

#endif
