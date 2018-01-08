#include "DSP_file.h"

struct DSP_file
{
    SNDFILE* originalWav;
    SNDFILE* newWav;
    SF_INFO oWavInfo;
    SF_INFO nWavInfo;
    sf_count_t readCounter;
};



SNDFILE* openWav (char* path, int mode, SF_INFO* fileInfo);
void fileOpenedCheck (SNDFILE* filePtr, char* path);

DSP_file* DSP_createFile (char* inputPath, char* outputPath)
{
    DSP_file* file = calloc (1, sizeof (DSP_file));   

    file->originalWav = openWav (inputPath, SFM_READ, &(file->oWavInfo));
    file->nWavInfo = file->oWavInfo;
    file->newWav = openWav (outputPath, SFM_WRITE, &(file->nWavInfo));
    file->readCounter = 0;

    return file;
}

void DSP_destroyFile (DSP_file* ptr)
{
    free (ptr);
}

SNDFILE* openWav (char* path, int mode, SF_INFO* fileInfo)
{
    SNDFILE* filePtr = sf_open (path, mode, fileInfo);
    fileOpenedCheck (filePtr, path);
    return filePtr;
}

void fileOpenedCheck (SNDFILE* filePtr, char* path)
{
    if (filePtr == NULL)
    {
        DSP_errorHandler (dsp_err_wav_not_open, path);
    }
}

int DSP_getSampleRate (DSP_file* file)
{
    return file->nWavInfo.samplerate;
}

int DSP_getNumChannels (DSP_file* file)
{
    return file->nWavInfo.channels;
}

void read (DSP_file* file, DSP_buffer* buffer)
{
    file->readCounter = sf_read_float (file->originalWav, DSP_getDataPtr(buffer), DSP_getBufferSize (buffer));
}

void write (DSP_file* file, DSP_buffer* buffer)
{
    int writeCount = sf_write_float (file->newWav, DSP_getDataPtr (buffer), file->readCounter);
    if (writeCount != file->readCounter)
    {
        DSP_errorHandler (dsp_err_write_error);
    }
}

void DSP_processFile (DSP_file* file, DSP_buffer* buffer, processFunc process, void* DSP_object)
{

    do
    {
        read (file, buffer);
        process (buffer, DSP_object);
        write (file, buffer);
    }
    while (file->readCounter != 0);
    DSP_clearBuffer;
}