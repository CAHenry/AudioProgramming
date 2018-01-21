#ifndef DSP_FILTER_H
#define DSP_FILTER_H

#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <math.h>

#include "DSP_file.h"
#include "DSP_windowing.h"

enum filterFunctions
{
    lowPass,
    highPass
};

typedef float (*filterFunc)(float ft, int index, int filterOrder);

float lowPassFunc (float ft, int index, int filterOrder);
float highPassFunc (float ft, int index, int filterOrder);

static const filterFunc filterFuncs[2] =
{
    lowPassFunc,
    highPassFunc
};

typedef struct DSP_filter DSP_filter;

DSP_filter* DSP_createFilter (int filterOrder, int cutoff, int sampleRate, int windowFlag, int responseFlag, float* delayLine);
void DSP_destroyFilter (DSP_filter* ptr);

void DSP_filterFile (DSP_file* file, float* buffer, int bufferSize, DSP_filter* filter);

#endif
