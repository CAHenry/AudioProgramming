#include "dsp_windowing.h"

float rectangularFunc (int index, int filterOrder)
{
    return 1;
}

float bartlettFunc (int index, int filterOrder)
{
    return 1.0 - (2.0 *  fabs (index - ((float)filterOrder / 2.0))) / (float) filterOrder;
}

float hanningFunc (int index, int filterOrder)
{
    return 0.54 - 0.46 * cos ((2.0 * M_PI * index) / filterOrder);
}

float hammingFunc (int index, int filterOrder)
{
    return 0.54 - 0.46 * cos ((2.0 * M_PI * index) / filterOrder);
}

float blackmanFunc (int index, int filterOrder)
{
    return 0.42 - 0.5 * cos ((2.0 * M_PI * index) / filterOrder) + 0.08 * cos (4.0 * M_PI * index / filterOrder);
}
