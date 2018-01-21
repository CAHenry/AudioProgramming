#include "DSP_filter.h"

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
        coeff = 1 - 2.0 * ft;
    }
    else
    {
        coeff = -sin (2.0 * M_PI * ft *(float) (n - orderShift)) / (M_PI * (n - orderShift));
    }
    
    return coeff;
}


struct DSP_filter
{
    int cutoff;
    int sampleRate;
    int windowFlag;
    int responseFlag;
    int filterOrder;
    float* coefficients;
    float* delayLine;
    int delayIndex;
};

// could be extended with a switch and enum to change response: LPF, HPF, BPF etc
void clearDelayLine (DSP_filter* filter);
void generateCoefficients (DSP_filter* filter);

DSP_filter* DSP_createFilter (int filterOrder, int cutoff, int sampleRate, int windowFlag, int responseFlag, float* delayLine)
{
    DSP_filter* filter = malloc(sizeof(DSP_filter));
    filter->filterOrder = filterOrder;
    filter->coefficients = calloc (filter->filterOrder + 1, sizeof (float));
    filter->cutoff = cutoff;
    filter->sampleRate = sampleRate;
    filter->windowFlag = windowFlag; // TODO:error checking bounds
    filter->responseFlag = responseFlag; // TODO:error checking bounds
    filter->delayLine = delayLine;
    filter->delayIndex = 0;

    clearDelayLine (filter);

    generateCoefficients (filter);
    
    return filter;
}

void clearDelayLine (DSP_filter* filter)
{
    for (int i = 0; i < filter->filterOrder; i++)
    {
        filter->delayLine[i] = 0;
    }
}

void generateCoefficients (DSP_filter* filter)

{
    float ft = (float) filter->cutoff / (float) filter->sampleRate;

    for (int i = 0; i < filter->filterOrder; i++)
    {
        float sampleImpulse = filterFuncs[filter->responseFlag] (ft, i, filter->filterOrder);
        float windowWeighting = windowFuncs[filter->windowFlag] (i, filter->filterOrder);
        filter->coefficients[i] =  sampleImpulse * windowWeighting;
    }
}

void DSP_destroyFilter (DSP_filter* ptr)
{
    free (ptr);
}

void filterCallback (float* data, int bufferSize, void* object)
{
    DSP_filter* filter = (DSP_filter*) object;
    float* h = filter->coefficients;
    float* d = filter->delayLine;
    int* index = &(filter->delayIndex);
    
    for (int i = 0; i < bufferSize; i ++)
    {
        int delayIndex = *index % filter->filterOrder;
        d [delayIndex] = data[i];
        data[i] = 0;
        for (int j = 0; j < filter->filterOrder; j++)
        {
            int coeffIndex = (*index + j) % filter->filterOrder;
            data[i] += d[coeffIndex] * h[j];
        }
        (*index)++;
    }
}

void DSP_filterFile (DSP_file* file, float* buffer, int bufferSize, DSP_filter* filter)
{
    //error check for sample rates.
    DSP_processFile (file, buffer, bufferSize, &filterCallback, filter);
}

