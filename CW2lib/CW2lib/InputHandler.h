#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include "DSP_file.h"
#include "DSP_windowing.h"
#include "DSP_filter.h"
#include "miscErrorChecks.h"

typedef struct config
{
    //file
    char* inputPath;
    char* outputPath;
    // filter
    int cutoff;
    int windowFlag;
    int filterFlag;
    //buffer
    int bufferSize;
} config;

void defaults (config* inputs);

void inputHandler (const int argc, char** argv, config* inputs);

void instructions ();

#endif