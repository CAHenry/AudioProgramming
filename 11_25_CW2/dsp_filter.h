#ifndef DSP_FILTER_H
#define DSP_FILTER_H

#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <math.h>

#include "dsp_windowing.h"
#include "dsp_error.h"

typedef enum
{
    lowPass,
    highPass
} filterFlag;

typedef float (*filterFunc)(float ft, int index, int filterOrder);

float lowPassFunc (float ft, int index, int filterOrder);
float highPassFunc (float ft, int index, int filterOrder);

static const filterFunc filterFuncs[2] =
{
    lowPassFunc,
    highPassFunc
};

typedef struct dsp_filter dsp_filter;

dsp_filter* dsp_createFilter (int filterOrder, int cutoff, int sampleRate, windowFlag window, filterFlag response, float* delayLine);
void dsp_destroyFilter (dsp_filter* ptr);

void dsp_filterAudioBlock (dsp_filter* filter, float* data, int bufferSize);

int dsp_flushDelayLine (dsp_filter* filter, float* data, int bufferSize);
#endif
