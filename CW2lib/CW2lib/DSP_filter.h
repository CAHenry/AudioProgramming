#ifndef DSP_FILTER_H
#define DSP_FILTER_H

#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <math.h>

#include "DSP_buffer.h"
#include "DSP_file.h"
#include "DSP_windowing.h"

typedef struct DSP_filter DSP_filter;

DSP_filter* DSP_createFilter (int filterOrder, int cutoff, int sampleRate, int windowFlag);
void DSP_destroyFilter (DSP_filter* ptr);

void DSP_filterFile (DSP_file* files, DSP_buffer* buffer, DSP_filter* fir);

#endif