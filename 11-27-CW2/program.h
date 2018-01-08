#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <unistd.h>
#include <sndfile.h>
#include <ctype.h>

#include "dsp_windowing.h"
#include "dsp_filter.h"

#include "errorHandling.h"

static const int g_maxBufferSize = 4096;

int handleOptionalArgs (const int argc, char** argv, windowFlag* window, filterFlag* response, int* bufferSize);
void handleCompulsoryArgs (int numOfOptionalArgs, const int argc, char** argv, char** inputPath, char** outputPath, int* cutoff);

void openWavs (SNDFILE** input, char* inputPath, SNDFILE** output, char* outputPath, SF_INFO* fileInfo);
void closeWavs (SNDFILE* input, SNDFILE* output);

#endif
