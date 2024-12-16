#include <lb/error.h>
#include <lb/generic.h>
#include <stdio.h>
#include <stdlib.h>


bool debug = false;

// terminate program execution with error
void error_exit(const char *format, ...) 
{
    fprintf(stderr, "%s: ", get_program_name());
    
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    fprintf(stderr, ".\n");
    if (debug) perror(NULL);
    exit(EXIT_FAILURE);
}
