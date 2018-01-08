#ifndef ERRORCHECKS_H
#define ERRORCHECKS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "DSP_error.h"
#include "DSP_file.h"

enum config_exit_code
{
    err_success,
    err_wav_not_mono = dsp_err_open_value,
    err_wrong_number_of_inputs
};

void errorHandler (int code, ...);

void wavMonoCheck (DSP_file* file);

#endif