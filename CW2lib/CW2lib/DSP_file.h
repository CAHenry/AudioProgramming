#ifndef DSP_FILE_H
#define DSP_FILE_H

#include <stdlib.h>
#include <sndfile.h>

#include "DSP_error.h"
#include "DSP_buffer.h"


//=============================================================

typedef struct DSP_file DSP_file;

typedef void (*processFunc)(DSP_buffer* buffer, void* DSP_object);

DSP_file* DSP_createFile (char* inputPath, char* outputPath);
void DSP_destroyFile (DSP_file* ptr);
int DSP_getSampleRate (DSP_file* file);
int DSP_getNumChannels (DSP_file* file);
void DSP_processFile (DSP_file* files, DSP_buffer* buffer, processFunc process, void* DSP_object);




#endif