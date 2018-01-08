//
//  main.c
//  printedSamplePlayer
//
//  Created by Christopher Hummersone on 22/08/2016.
//  Copyright © 2016 Christopher Hummersone. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>

#define FRAMES_PER_BUFFER 512   // buffer size
#define FS 48000                // sample rate

// minimum function
#define min(a,b) \
    ({ __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })

// Callback function passed to portaudio to play audio file
PaStreamCallback playCallback;

// Set up output device
void getOutputParameters(PaStreamParameters *p);

// Function for re-/setting buffer
int resetBuffer(float * buffer, int start, int end);

// error numbers
enum ERR_MSGS {
    NO_ERROR,
    ERROR_PA,
    BAD_COMMAND_LINE,
    NO_MEMORY
};

// structure containing audio data from stdin
struct audioInfo {
    float * buffer;
    unsigned int idx;
    size_t numSamples;
};

int main(int argc, const char * argv[]) {
    
    int err = NO_ERROR;                     // Error number
    PaError paerr = paNoError;              // Portaudio error number
    struct audioInfo audioData;             // Audio buffer data
    char * line = NULL;                     // Space for each line of stdin
    size_t numChars = 0;                    // Number of chars returned by getline
    int numLinesRead = 0;                   // Number of lines read from stdin
    PaStream * stream = NULL;               // Audio output stream
    PaStreamParameters outputParameters;    // Audio output stream parameters
    
    // program takes no arguments
    if ( argc > 1 ) {
        // handle this error
        err = BAD_COMMAND_LINE;
        goto cleanup;
    }
    
    // initial allocation of audio data
    audioData.idx = 0;
    audioData.numSamples = FRAMES_PER_BUFFER;
    audioData.buffer = calloc(FRAMES_PER_BUFFER, sizeof(float));
    err = resetBuffer(audioData.buffer, 0, (int)audioData.numSamples);
    if ( err ) goto cleanup; // check memory was allocated
    
    // go through lines from stdin
    while ( getline(&line, &numChars, stdin) > 0 ) {
        if (*line == '\n') // break loop on newline
            break;
        if ( numLinesRead >= audioData.numSamples ) {
            // if buffer is not big enough then expand it
            // by FRAMES_PER_BUFFER samples
            audioData.numSamples += FRAMES_PER_BUFFER;
            audioData.buffer =
                realloc(audioData.buffer,audioData.numSamples*sizeof(float));
            err = resetBuffer(
                audioData.buffer,
                numLinesRead,
                (int)audioData.numSamples
            );
            if ( err ) goto cleanup; // check memory was allocated
        }
        // convert char to float and write to buffer
        audioData.buffer[numLinesRead] = atof(line);
        // we've read a line
        numLinesRead++;
    }
    
    // initialise portaudio
    paerr = Pa_Initialize();
    if ( paerr ) {
        err = ERROR_PA;
        goto cleanup;
    }
    
    // Set up output device
    getOutputParameters(&outputParameters);
    outputParameters.sampleFormat = paFloat32; // specify output format
    outputParameters.channelCount = 1; // mono
    
    // open stream for outputting audio file via callback
    
    paerr = Pa_OpenStream(
        &stream,
        NULL, /* no input */
        &outputParameters,
        FS,
        FRAMES_PER_BUFFER,
        paClipOff,
        playCallback,
        &audioData
    );
    if ( paerr ) {
        err = ERROR_PA;
        goto cleanup;
    }
    
    // start playing
    paerr = Pa_StartStream(stream);
    if ( paerr ) {
        err = ERROR_PA;
        goto cleanup;
    }
    
    // wait for audio file to finish playing or for user to press space bar and
    // enter
    printf("Now playing...\n");
    while( Pa_IsStreamActive(stream) ) Pa_Sleep(100);
    
    // Finished playing
    printf("Finished!\n");
    
    goto cleanup;
    
cleanup:
    
    // make sure all the toys are put away before exit
    
    // close stream
    paerr = Pa_CloseStream(stream);
    
    // terminate portaudio
    Pa_Terminate();
    
    // free memory
    free(audioData.buffer);
    free(line);
    
    // terminate portaudio
    Pa_Terminate();
    
    // display error message
    if ( err ) {
        switch ( err ) {
            case ERROR_PA:
                puts("An error occured while using the portaudio stream.\n");
                printf("Error number: %d\n", paerr);
                printf("Error message: %s\n", Pa_GetErrorText(paerr));
            case BAD_COMMAND_LINE:
                puts("Error: provide samples via stdin.\n");
            case NO_MEMORY:
                puts("Error: unable to allocate memory.\n");
            default:
                puts("An unknown error occurred.\n");
        }
    }
    
    return err;
}

// Callback function passed to portaudio to play audio file
int playCallback(
    const void *inputBuffer,
    void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData
) {
    // cast inputs to correct data type
    struct audioInfo * audioData =
        (struct audioInfo *) userData;   // audio data input from stdin
    float *out = (float*)outputBuffer;   // output buffer
    
    // avoid unused variable warnings
    (void) inputBuffer;
    (void) timeInfo;
    (void) statusFlags;
    
    unsigned long remaining =
        (audioData->numSamples-audioData->idx); // number of frames remaining
    if ( remaining > 0 ) { // If data to read
        // fill output buffer
        unsigned long numFrames = min(remaining,framesPerBuffer);
        for ( int n = 0; n < numFrames; n++ ) {
            *out++ = (audioData->buffer)[audioData->idx]; // write to output
            (audioData->idx)++; // increment current position
        }
        return paContinue; // continue playing
    }
    else
        return paComplete; // finished playing
}

// Set up output device
void getOutputParameters(PaStreamParameters *p) {
    
    // Use the default output device
    PaDeviceIndex id = Pa_GetDefaultOutputDevice();
    const PaDeviceInfo *info = Pa_GetDeviceInfo(id);
    const PaHostApiInfo *hostapi = Pa_GetHostApiInfo(info->hostApi);
    printf("Opening audio output device [%s] %s\n",hostapi->name, info->name);
    
    // Set the stream parameters
    p->device = id;
    p->suggestedLatency = Pa_GetDeviceInfo(p->device)->defaultLowOutputLatency;
    p->hostApiSpecificStreamInfo = NULL;
}

// Function for re-/setting buffer
int resetBuffer(float* buffer, int start, int end) {
    int err = 0;
    if ( buffer == NULL ) {
        // check memory was allocated
        err = NO_MEMORY;
        printf("Insufficient memory for audio data.\n");
    }
    else {
        // reset buffer
        for (int n = start; n < end; n++) {
            buffer[n] = 0.0;
        }
    }
    return err;
}
