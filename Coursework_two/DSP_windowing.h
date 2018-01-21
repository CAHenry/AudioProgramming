#ifndef DSP_WINDOWING_H
#define DSP_WINDOWING_H
#define _USE_MATH_DEFINES
#include <math.h>

enum windowFunctions
{
    rectangular,
    barlett,
    hanning,
    hamming,
    blackman
};

typedef float (*windowFunc)(int index, int filterOrder);

float rectangularFunc (int index, int filterOrder);
float bartlettFunc (int index, int filterOrder);
float hanningFunc (int index, int filterOrder);
float hammingFunc (int index, int filterOrder);
float blackmanFunc (int index, int filterOrder);

static const windowFunc windowFuncs[5] =
{
    rectangularFunc,
    bartlettFunc,
    hanningFunc,
    hammingFunc,
    blackmanFunc,
};

#endif
