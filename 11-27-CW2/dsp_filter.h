#ifndef DSP_FILTER_H
#define DSP_FILTER_H

#define _USE_MATH_DEFINES
#include <stdlib.h> // required for memory allocations and free
#include <string.h> // required for memset() when clearing delay line
#include <math.h> // required for M_PI define.

#include "dsp_windowing.h" // required to call window functions
#include "dsp_error.h" // required for dsp_errorHandler

/*
 * Flag used to set the reponse of a filter.
 */
typedef enum
{
    lowPass,
    highPass,
    num_responses
} filterFlag;

/*
 * Finite impulse response (FIR) filter.
 */
typedef struct dsp_filter dsp_filter;

/*
 * Create filter of Order <filterOrder> and cutoff frequency <cutoff>. The window function and filter reponse are set by <window> and <response>.
 * The filter must also be given a pointer to an array of floats sized <filterOrder + 1>. The sample rate being used must also be provided.
 * For example to create a 126th order low pass filter at 44.1kHz using a hamming window:
 *
 * int filterOrder = 126;
 * float delayLine [filterOrder + 1] = {0};
 * dsp_createFilter (filterOrder, 5000, lowPass, hamming, delayLine, 44100;
 */
dsp_filter* dsp_createFilter (int filterOrder, int cutoff, filterFlag response, windowFlag window, float* delayLine, int sampleRate);

/*
 * Destroy a filter and free the memory used.
 */
void dsp_destroyFilter (dsp_filter* ptr);

/*
 * Use the FIR filter <filter> to filter a block of audio of a size <blockSize> found at <data>.
 */
void dsp_filterAudioBlock (dsp_filter* filter, float* data, long blockSize);
#endif
