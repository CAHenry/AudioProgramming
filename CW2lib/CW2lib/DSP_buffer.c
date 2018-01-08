#include "DSP_buffer.h"

typedef struct DSP_buffer
{
    int size;
    int index;
    float* data;
} DSP_buffer;

DSP_buffer* DSP_createBuffer (unsigned int bufferSize)
{
    if (bufferSize > maxBufferSize)
    {
        DSP_errorHandler (dsp_war_buffer_too_big, bufferSize, maxBufferSize);
        bufferSize = maxBufferSize;
    }
    else if (DSP_isPowerOfTwo (bufferSize) == false)
    {
        DSP_errorHandler (dsp_war_buffer_not_pow_two, bufferSize, maxBufferSize);
        bufferSize = maxBufferSize;
    }

    DSP_buffer* buffer = malloc (sizeof (DSP_buffer));
    buffer->index = 0;
    buffer->size = bufferSize;
    buffer->data = _alloca (sizeof (double) * buffer->size);
    DSP_clearBuffer (buffer);
    return buffer;
}

//DSP_buffer* DSP_createBuffer (unsigned int bufferSize)
//{
//    if (bufferSize > maxBufferSize)
//    {
//        DSP_errorHandler (dsp_war_buffer_too_big, bufferSize);
//        bufferSize = maxBufferSize;
//    }
//    else if (DSP_isPowerOfTwo (bufferSize))
//    {
//        DSP_errorHandler (dsp_war_buffer_not_pow_two, bufferSize);
//        bufferSize = maxBufferSize;
//    }
//
//    DSP_buffer* buffer;
//    buffer->index = 0;
//    buffer->size = bufferSize;
//    DSP_clearBuffer (buffer);
//    return buffer;
//}

void DSP_clearBuffer (DSP_buffer* buffer)
{
    for (int i = 0; i < buffer->size; i++)
    {
        buffer->data[i] = 0;
    }
}

float* DSP_getDataPtr (DSP_buffer * buffer)
{
    return buffer->data;
}

int DSP_getBufferSize (DSP_buffer * buffer)
{
    return buffer->size;
}
