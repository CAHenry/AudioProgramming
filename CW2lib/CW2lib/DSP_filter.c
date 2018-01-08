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


struct DSP_filter
{
    int cutoff;
    int sampleRate;
    int windowFlag;
    int responseFlag;
    int filterOrder;
    float* coefficients;
    float* delayLine;
};

// could be extended with a switch and enum to change response: LPF, HPF, BPF etc
void clearDelayLine (DSP_filter* filter);
void generateCoefficients (DSP_filter* filter);

DSP_filter* DSP_createFilter (int filterOrder, int cutoff, int sampleRate, int windowFlag)
{
    DSP_filter* filter = malloc(sizeof(DSP_filter));
    filter->filterOrder = filterOrder;
    filter->coefficients = calloc (filter->filterOrder + 1, sizeof (float));
    filter->cutoff = cutoff;
    filter->sampleRate = sampleRate;
    filter->windowFlag = windowFlag; // error checking cant be > 4
    filter->delayLine = _alloca ((filter->filterOrder + 1) * sizeof (float));

    clearDelayLine (filter);

    generateCoefficients (filter);
    
    return filter;
}

void clearDelayLine (DSP_filter* filter)
{
    for (int i = 0; i < filter->filterOrder + 1; i++)
    {
        filter->delayLine[i] = 0;
    }
}

void generateCoefficients (DSP_filter* filter)

{
    float ft = (float) filter->cutoff / (float) filter->sampleRate;
    int orderShift = filter->filterOrder / 2;

    for (int i = 0; i < filter->filterOrder; i++)
    {
        if (i != orderShift)
        {
            filter->coefficients[i] = sin (2.0*M_PI*ft*(float) (i - orderShift)) / (M_PI * (i - orderShift));
        }
        else
        {
            filter->coefficients[i] = 2.0 * ft;
        }

        filter->coefficients[i] *= windows[filter->windowFlag] (i, filter->filterOrder);
    }
}

void DSP_destroyFilter (DSP_filter* ptr)
{
    free (ptr);
}

void process_filter (DSP_buffer* buffer, DSP_filter* filter)
{
    /* do filter things*/
}

void DSP_filterFile (DSP_file* file, DSP_buffer* buffer, DSP_filter* filter)
{
    //error check for sample rates.
    DSP_processFile (file, buffer, process_filter, filter);
}

