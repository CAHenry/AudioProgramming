/**************************************************************************
 * FILE NAME: dsp_filter   .c                                             *
 *                                                                        *
 * PURPOSE: FIR filters for filtering audio                               *
 *                                                                        *
 * EXTERNAL VARIABLES:                                                    *
 * <dsp_windowing.h>                                                      *
 * Name      | Type       | Description                                   *
 * windows[] | windowFunc | Array containing pointers to window functions *
 *                                                                        *
 * REQUIREMENTS: dsp_windowing.h, dsp_error.h, math.h, stdlib.h, string.h *
 *************************************************************************/
#include "dsp_filter.h"

 /*************************************************************************************************************************************/

/*______________dsp_filter______________
 * contains all information relevant to an FIR filter:
 *     - The filter order <filterOrder>
 *     - The cutoff frequency <cutoff>
 *     - The response of the filter, lowPass or highPass <response>
 *     - the window function used <window>
 *     - the location of the delay line <delayLine>
 *     - the size of the delay line <delayLength>
 *     - the impulse reponse of the filter <coefficients>
 *     - the sample rate being used <sampleRate>
 */
struct dsp_filter
{
    int filterOrder;
    int cutoff;
    filterFlag response;
    windowFlag window;
    float* delayLine;
    int delayLength;
    int delayIndex;
    float* coefficients;
    int sampleRate;
};

/*************************************************************************************************************************************/

/* Function prototype for dsp_createFilter */
void generateCoefficients (dsp_filter* filter);

/*______________dsp_createFilter______________
 * Creates an FIR filter of order <filterOrder>. Sets the cutoff frequency <cutoff>, response <response> and window function <window>
 * The filter must be provided with a pointer to a delay line of size (<filterOrder> + 1) <delayLine>. The sample rate <sampleRate>
 * is also required to calculate the impulse response of the filter.
 */
dsp_filter* dsp_createFilter (int filterOrder, int cutoff, filterFlag response, windowFlag window, float* delayLine, int sampleRate)
{
    /* create filter and allocate memory */
    dsp_filter* filter;
    if ( ! (filter = malloc (sizeof (dsp_filter)))) // if null memory not allocated
    {
        dsp_errorHandler (dsp_err_memory_not_allocated);
    }
    
    /* set parameters */

    filter->filterOrder = filterOrder;
    filter->cutoff  = cutoff;
    
    if (response < num_responses) // check response is recognised
    {
        filter->response = response;
    }
    else
    {
        dsp_errorHandler(dsp_err_filter_response_not_recognised, response); 
    }
    
    if (window < num_windows) // check window is recognised
    {
        filter->window = window;
    }
    else
    {
        dsp_errorHandler(dsp_err_window_function_not_recognised, window);
    }
    
    /* Zero the delay line and delay index */
    filter->delayLine = delayLine;
    filter->delayLength = filterOrder + 1;
    memset(filter->delayLine, 0, filter->delayLength * sizeof(float));
    filter->delayIndex = 0;

    /* Zero initialise the array in which the coefficients are stored */
    if ( ! (filter->coefficients = calloc (filter->delayLength, sizeof (float)))) // if null memory not allocated
    {
        dsp_errorHandler (dsp_err_memory_not_allocated);
    }
    
    /* generate the impulse response */
    filter->sampleRate = sampleRate;
    generateCoefficients (filter);
    
    return filter;
}

/*************************************************************************************************************************************/


/*______________dsp_destroyFilter______________
* Destroy a filter and free the memory used. Also free the memory for the impulse response
*/
void dsp_destroyFilter (dsp_filter* ptr)
{
    free (ptr->coefficients);
    free (ptr);
}

/*************************************************************************************************************************************/

/*______________dsp_filterAudioBlock______________
 * Use the FIR filter <filter> to filter a block of audio of a size <blockSize> found at <data>.
 *
 *                 +-----+             +-----+             +-----+
 *                 |     |             |     |             |     |
 * >--x[n]---------+ z-1 +-------------+ z-1 +-------------+ z-1 +-------+
 *           |     |     |       |     |     |       |     |     |       |
 *           |     +-----+       |     +-----+       |     +-----+       |
 *           |                   |                   |                   |
 *       \-------/           \-------/           \-------/           \-------/
 *        \h[0] /             \h[1] /             \h[2] /             \h[3] /
 *         \   /               \   /               \   /               \   /
 *          \ /                 \ /                 \ /                 \ /
 *           |                   |                   |                   |
 *           |                   |                   |                   |
 *           |                  .-.                 .-.                 .-.
 *           +-----------------(sum)---------------(sum)---------------(sum)---y[n]->
 *                              '-'                 '-'                 '-'
 * For each sample:
 * 1. x[n] is read from the buffer into the delay line (in the diagram above this is 4 samples long).
 * 2. The buffer value is then set to zero before y[n] is calculated
 * 3. y[n] is calculated by summing each value of the delay line multiplied by h[n] (coefficients[] below)
 * 4. increment the delay line index
 */

void dsp_filterAudioBlock (dsp_filter* filter, float* data, long numSamples)
{
    for (int i = 0; i < numSamples; i ++)
    {
        filter->delayLine [filter->delayIndex % filter->delayLength] = data[i]; //1
        
        data[i] = 0; //2
        for (int j = 0; j < filter->delayLength; j++)
        {
            /* values are summed starting from the most delayed sample until the current sample. For this reason the impulse response
             * coefficients are iterated backwards. */
            data[i] += filter->delayLine[(filter->delayIndex + j + 1) % filter->delayLength] * filter->coefficients[filter->filterOrder - j]; //3
        }

        filter->delayIndex++; // 4
    }
}

/*************************************************************************************************************************************/

float lowPassFunc (float ft, int n, int filterOrder)
{
    int orderShift = filterOrder / 2.0;
    float coeff;
    if (n != orderShift)
    {
        coeff = sin (2.0 * M_PI * ft * (float) (n - orderShift)) / (M_PI * (n - orderShift));
    }
    else
    {
        coeff = 2.0 * ft;
    }
    
    return coeff;
}

/*************************************************************************************************************************************/

float highPassFunc (float ft, int n, int filterOrder)
{
    if (filterOrder % 2 != 0)
    {
        dsp_errorHandler (dsp_err_hpf_not_even);
    }
    
    int orderShift = filterOrder / 2.0;
    float coeff;
    if (n != orderShift)
    {
        coeff = - sin (2.0 * M_PI * ft * (float) (n - orderShift)) / (M_PI * (n - orderShift));
    }
    else
    {
        coeff = 1.0 - (2.0 * ft);

    }
    
    return coeff;
}

/*************************************************************************************************************************************/

//TODO: window functions extern declaration?

typedef float (*filterFunc)(float ft, int index, int filterOrder);

const filterFunc filterFuncs [num_responses] =
{
    lowPassFunc,
    highPassFunc,
};

/*************************************************************************************************************************************/

void generateCoefficients (dsp_filter* filter)
{
    float ft = (float) filter->cutoff / (float) filter->sampleRate;
    for (int i = 0; i < filter->delayLength; i++)
    {
        float sampleImpulse = filterFuncs[filter->response] (ft, i, filter->filterOrder);
        float windowWeighting = windowFuncs[filter->window] (i, filter->filterOrder);
        filter->coefficients[i] =  sampleImpulse * windowWeighting;
    }
}

/*************************************************************************************************************************************/

/*
 FIR notes
 
 Convolution x(t) * f(t)
 
 126 samples taken, each multiplied by a coefficient, all of which are summed.
 
 h(n) = (1 / (pi * n))sin(wc*n) ideal low pass filter - unwindowed and non-causal
 
 h(n) = (1 / (pi * n))sin(wc*n + M/2) - causal
 
 h(0) = (1 / pi)cos(wc*n)*wc  l'hospital? ideal low pass filter - unwindowed
 cos(0) = 1
 h(0) = wc / pi
 
 windowing
 h(n) = w(n) * h(n)infinite - causal and windowed
 */
