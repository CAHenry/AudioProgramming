#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <unistd.h>
#include <sndfile.h>

#include "dsp_filter.h"

#include "errorHandling.h"

int optionalArgs (const int argc, char** argv, windowFlag* window, filterFlag* response, int* bufferSize);
void compulsoryArgs (int numOfOptionalArgs, const int argc, char** argv, char** inputPath, char** outputPath, int* cutoff);

void openWavs (SNDFILE** input, char* inputPath, SNDFILE** output, char* outputPath, SF_INFO* fileInfo);
void closeWavs (SNDFILE* input, SNDFILE* output);

#endif
