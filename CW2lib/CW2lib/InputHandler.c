#include "InputHandler.h"

void defaults (config* inputs)
{
    inputs->windowFlag = hanning;
    inputs->filterFlag = 0; // TODO: CHANGE WHEN ENUM IS MADE
    inputs->bufferSize = 2048;
}

optionalArgs (const int argc, char** argv, config* inputs);


void inputHandler (const int argc, char** argv, config* inputs)
{
    //optionalArgs (argc, argv, inputs)


    if (argc == 1)
    {
        instructions ();
        exit (0);
    }
    else if (argc != 4)
    {
        errorHandler (err_wrong_number_of_inputs, argc - 1);
    }
    else
    {
        inputs->inputPath = argv[1]; // TODO: error checking for isString
        inputs->outputPath = argv[2]; // TODO: error checking for isString
        inputs->cutoff = atoi (argv[3]); // TODO: error checking for isInteger
    }
}

//optionalArgs (const int argc, char** argv, config* inputs)
//{
//    int opt;
//    while ((opt = getopt (argc, argv, "w:f:b:")) != -1)
//    {
//        switch (opt)
//        {
//        case 'w':
//            inputs->windowFlag = optarg;
//        case 'f':
//            inputs->filterFlag = optarg;
//        case 'b':
//            inputs->bufferSize = optarg;
//        }
//    }
//}

void borderBS ()
{

}

void instructions ()
{
    borderBS ();
}
