#include "dsp_filter.h"

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

float highPassFunc (float ft, int n, int filterOrder)
{
    // TODO: error handling filter order must be even
    int orderShift = filterOrder / 2.0;
    float coeff;
    if (n != orderShift)
    {
        coeff = - (sin (2.0 * M_PI * ft * (float) (n - orderShift)) / (M_PI * (n - orderShift))); // TODO: DOESNT WORK
    }
    else
    {
        coeff = 1 - (2.0 * ft);

    }
    
    return coeff;
}


struct dsp_filter
{
    int cutoff;
    int sampleRate;
    windowFlag window;
    filterFlag response;
    int filterOrder;
    float* coefficients;
    float* delayLine;
    int delayIndex;
};

// could be extended with a switch and enum to change response: LPF, HPF, BPF etc
void clearDelayLine (dsp_filter* filter);
void generateCoefficients (dsp_filter* filter);

dsp_filter* dsp_createFilter (int filterOrder, int cutoff, int sampleRate, windowFlag window, filterFlag response, float* delayLine)
{
    dsp_filter* filter;
    if ( ! (filter = malloc (sizeof (dsp_filter))))
    {
        dsp_errorHandler (dsp_err_memory_not_allocated);
    }
    
    filter->cutoff = cutoff;
    filter->sampleRate = sampleRate;
    filter->window = window; // TODO:error checking bounds
    filter->response = response; // TODO:error checking bounds

    filter->filterOrder = filterOrder;
    
    if ( ! (filter->coefficients = calloc (filter->filterOrder + 1, sizeof (float))))
    {
        dsp_errorHandler (dsp_err_memory_not_allocated);
    }
    generateCoefficients (filter);
    
    filter->delayLine = delayLine;
    filter->delayIndex = 0;
    
    clearDelayLine (filter);
    
    return filter;
}

void clearDelayLine (dsp_filter* filter)
{
    for (int i = 0; i < filter->filterOrder; i++)
    {
        filter->delayLine[i] = 0;
    }
}

void generateCoefficients (dsp_filter* filter)

{
    float ft = (float) filter->cutoff / (float) filter->sampleRate;

    for (int i = 0; i < filter->filterOrder; i++)
    {
        float sampleImpulse = filterFuncs[filter->response] (ft, i, filter->filterOrder);
        float windowWeighting = windowFuncs[filter->window] (i, filter->filterOrder);
        filter->coefficients[i] =  sampleImpulse * windowWeighting;
    }
}

void dsp_destroyFilter (dsp_filter* ptr)
{
    free (ptr);
}

void dsp_filterAudioBlock (dsp_filter* filter, float* data, int bufferSize)
{
    for (int i = 0; i < bufferSize; i ++)
    {
        filter->delayLine [filter->delayIndex % filter->filterOrder] = data[i];
        data[i] = 0;
        
        for (int j = 0; j < filter->filterOrder; j++)
        {
            data[i] += filter->delayLine[(filter->delayIndex + j) % filter->filterOrder] * filter->coefficients[j];
        }
        
        filter->delayIndex++;
    }
}

int dsp_flushDelayLine (dsp_filter* filter, float* buffer, int bufferSize)
{
    int count = 0;
    while (count < filter->filterOrder + 1 && count < bufferSize)
    {
        buffer[count] = filter->delayLine[filter->delayIndex++ % filter->filterOrder];
        count++;
    }
    return count;
}

