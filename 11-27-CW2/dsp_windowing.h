#ifndef DSP_WINDOWING_H
#define DSP_WINDOWING_H
#include <math.h> // required for cos(), fabs() and M_PI

/*
 * The flags used to access different window functions.
 */
typedef enum
{
    rectangular,
    barlett,
    hanning,
    hamming,
    blackman,
    num_windows
} windowFlag;

/*
 * A function pointer to be called in order to calculate the value
 * at index <index> of a window function of length <length>
 */
typedef float (*windowFunc)(int index, int length);

/*
 * An array containing function pointers to different window functions
 * of type windowFunc. Access different window functions using the
 * windowFlag and this array.
 */
extern const windowFunc windowFuncs[num_windows];
#endif
