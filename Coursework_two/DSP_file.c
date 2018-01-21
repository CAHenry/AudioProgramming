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

void DSP_destroyFile (DSP_file* file)
{
    sf_close(file->newWav);
    sf_close(file->originalWav);
    free (file);
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

void DSP_read (DSP_file* file, float* buffer, int bufferSize)
{
    file->readCounter = sf_readf_float (file->originalWav, buffer, bufferSize / DSP_getNumChannels(file));
}

void DSP_write (DSP_file* file, float* buffer)
{
    sf_count_t writeCount = sf_writef_float (file->newWav, buffer, file->readCounter);
    if (writeCount != file->readCounter)
    {
        DSP_errorHandler (dsp_err_write_error);
    }
}

void DSP_processFile (DSP_file* file, float* buffer, int bufferSize, callbackFunc callback, void* DSP_object)
{
    do
    {
        DSP_read (file, buffer, bufferSize);
        callback (buffer, (int) file->readCounter, DSP_object);
        DSP_write (file, buffer);
    }
    while (file->readCounter != 0);
}
