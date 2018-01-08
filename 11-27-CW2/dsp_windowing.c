/**********************************************************************
 * FILE NAME: dsp_windowing.c                                         *
 *                                                                    *
 * PURPOSE: providing window functions for DSP algorithms             *
 *                                                                    *
 * EXTERNAL VARIABLES: none                                           *
 *                                                                    *
 * REQUIREMENTS: dsp_windowing.h, math.h                              *
 *********************************************************************/

#include "dsp_windowing.h"

/**************************************************************************************************/

/*
 * returns the value for a rectangular window function at index <index> for a function of length <length>
 */
float rectangularFunc (int index, int length)
{
    return 1.0;
}

/**************************************************************************************************/

/*
 * returns the value for a Bartlett window function at index <index> for a function of length <length>
 */
float bartlettFunc (int index, int length)
{
    return 1.0 - (2.0 *  fabs (index - ((float) length / 2.0))) / (float) length;
}

/**************************************************************************************************/

/*
 * returns the value for a Hanning window function at index <index> for a function of length <length>
 */
float hanningFunc (int index, int length)
{
    return 0.5 - 0.5 * cos ((2.0 * M_PI * (float) index) / (float) length);
}

/**************************************************************************************************/

/*
 * returns the value for a Hamming window function at index <index> for a function of length <length>
 */
float hammingFunc (int index, int length)
{
    return 0.54 - 0.46 * cos ((2.0 * M_PI * (float) index) / (float) length);
}

/**************************************************************************************************/

/*
 * returns the value for a Blackman window function at index <index> for a function of length <length>
 */
float blackmanFunc (int index, int length)
{
    return 0.42 - 0.5 * cos ((2.0 * M_PI * (float) index) / (float) length) + 0.08 * cos (4.0 * M_PI * (float) index / (float) length);
}

/**************************************************************************************************/

/*
 * array containing function pointers to different window functions which can be accessed by the window flags
 */
const windowFunc windowFuncs[num_windows] =
{
    rectangularFunc,
    bartlettFunc,
    hanningFunc,
    hammingFunc,
    blackmanFunc,
};
