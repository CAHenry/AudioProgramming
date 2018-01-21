#include "InputHandler.h"

void defaults (config* inputs)
{
    inputs->windowFlag = hanning;
    inputs->filterFlag = lowPass; 
}

int optionalArgs (const int argc, char** argv, config* inputs);

void inputHandler (const int argc, char** argv, config* inputs)
{
    int numOptionArgs = optionalArgs (argc, argv, inputs);
    int numArgs = argc - numOptionArgs;
    
    if (numArgs == 0)
    {
        instructions ();
        exit (0);
    }
    else if (numArgs != 3)
    {
        errorHandler (err_wrong_number_of_inputs, numArgs);
    }
    else
    {
        inputs->inputPath = argv[numOptionArgs]; // TODO: error checking for isString
        inputs->outputPath = argv[numOptionArgs + 1]; // TODO: error checking for isString
        inputs->cutoff = atoi (argv[numOptionArgs + 2]); // TODO: error checking for isInteger
    }
}

int optionalArgs (const int argc, char** argv, config* inputs)
{
    int opt;
    while ((opt = getopt (argc, argv, "w:f:")) != -1)
    {
        switch (opt)
        {
        case 'w':
                inputs->windowFlag = atoi(optarg); // TODO: error checking within bounds
                break;
        case 'f':
                inputs->filterFlag = atoi(optarg); // TODO: error checking within bounds
                break;
        case '?':
                errorHandler (err_option_not_recognised, optopt);
                break;
        }
    }
    
    return optind;
}

void borderBS ()
{

}

void instructions ()
{
    borderBS ();
}
