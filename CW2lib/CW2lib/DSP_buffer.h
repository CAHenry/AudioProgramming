#ifndef DSP_BUFFER_H
#define DSP_BUFFER_H

#include "DSP_error.h"

#define maxBufferSize 2048

typedef struct DSP_buffer DSP_buffer;

//typedef struct buffer_s
//{
//    int size;
//    int index;
//    double data[maxBufferSize];
//} DSP_buffer;

DSP_buffer* DSP_createBuffer (unsigned int bufferSize);

void DSP_clearBuffer (DSP_buffer* buffer);

float* DSP_getDataPtr (DSP_buffer* buffer);

int DSP_getBufferSize (DSP_buffer* buffer);

#endif