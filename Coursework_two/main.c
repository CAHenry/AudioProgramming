/*
The command line inputs
Name of input
Name of output
Cut-off frequency

No arguments prints instructions for use. 

Input and output both mono

Anything non-mono result in message and programme terminating without
audio being processed.

Low pass filter, 126th order linear-phase FIR filter in time domain

Errorchecking for all I/O operations and mem allocs

*/

/*
Useful libsndfile stuff:

int  sf_format_check (const SF_INFO *info) ;
This function allows the caller to check if a set of parameters in the SF_INFO struct is valid before calling sf_open (SFM_WRITE).

sf_format_check returns TRUE if the parameters are valid and FALSE otherwise.

int         sf_error        (SNDFILE *sndfile) ;
This function returns the current error number for the given SNDFILE. The error number may be one of the following:

enum
{   SF_ERR_NO_ERROR             = 0,
SF_ERR_UNRECOGNISED_FORMAT  = 1,
SF_ERR_SYSTEM               = 2,
SF_ERR_MALFORMED_FILE       = 3,
SF_ERR_UNSUPPORTED_ENCODING = 4
} ;
or any one of many other internal error values. Applications should only test the return value against error values defined in <sndfile.h>
as the internal error values are subject to change at any time. For errors not in the above list, the function
sf_error_number() can be used to convert it to an error string.

const char* sf_strerror     (SNDFILE *sndfile) ;
const char* sf_error_number (int errnum) ;
The error functions sf_strerror() and sf_error_number() convert the library's internal error enumerations into text strings.
*/

/*
FIR notes

Convolution x(t) * f(t)

126 samples taken, each multiplied by a coefficient, all of which are summed.

h(n) = (1 / (pi * n))sin(wc*n) ideal low pass filter - unwindowed and non-causal

h(n) = (1 / (pi * n))sin(wc*n + M/2) - causal

h(0) = (1 / pi)cos(wc*n)*wc  l'hospital? ideal low pass filter - unwindowed
cos(0) = 1
h(0) = wc / pi

windowing
h(n) = w(n) * h(n)infinite - causal and windowed
*/

#include "DSP_file.h"
#include "DSP_filter.h"
#include "DSP_error.h"

#include "miscErrorChecks.h"
#include "InputHandler.h"


int main (const int argc, char** argv)
{
    const int filterOrder = 126;
    const int bufferSize = 512;
    float buffer[bufferSize] = {0};
    float delayLine[filterOrder + 1] = {0};
    
    config values = {0};
    defaults (&values);
    inputHandler (argc, argv, &values);

    DSP_file* file = DSP_createFile (values.inputPath, values.outputPath);

    wavMonoCheck (file);

    DSP_filter* filter = DSP_createFilter (filterOrder, values.cutoff, DSP_getSampleRate (file), hanning, lowPass, delayLine);
 
    DSP_filterFile (file, buffer, bufferSize, filter);

    DSP_destroyFile (file);
    DSP_destroyFilter (filter);

    errorHandler (err_success);
}
