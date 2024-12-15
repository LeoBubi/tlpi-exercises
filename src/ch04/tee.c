/* 
Implement tee using I/O system calls. Implement the –a command-line 
option, which causes tee to append text to the end of a file if it 
already exists.
*/

#define PROGRAM_NAME "tee"
#define VERSION      "0.1"
#define RELEASE_DATE "2024-12-15"
#define AUTHORS      "Leonardo Brugnano"

#define _GNU_SOURCE 1
#include <fcntl.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>


#define help() do { \
    fprintf(stdout, \
        "Usage: "PROGRAM_NAME" [OPTION]... [FILE]...\n" \
        "Copy standard input to each FILE, and also to standard output.\n" \
        "\n" \
        "  -a, --append     append to the given FILEs, do not overwrite\n" \
        "\n" \
        "    --help      display help and exit\n" \
        "    --version   output version information and exit\n" \
        "\n"); \
    exit(EXIT_SUCCESS); \
} while (0)

#define version() do { \
    fprintf(stdout, \
        PROGRAM_NAME" version "VERSION" ("RELEASE_DATE")\n" \
        "Written by "AUTHORS); \
    exit(EXIT_SUCCESS); \
} while(0)

int main(int argc, char **argv)
{
    unsigned int oflag = O_TRUNC;

    /* read command-line options */
    const char optstring[] = "a";
    const struct option longopts[] = {
        {"append", no_argument, NULL, 'a'},
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {0, 0, 0, 0}
    };

    int optchar;
    while ((optchar = getopt_long(argv, argv, optstring, longopts, NULL)) == -1)
        switch (optchar) {
            case 'a':
                oflag = O_APPEND;
                break;
            case 'h':
                help();
            case 'v':
                version();
            default:
                fprintf(stderr, 
                    PROGRAM_NAME": invalid option -- '%c'\n"
                    "Try '"PROGRAM_NAME" --help' for more information"
                    , (char)optopt);
                exit(EXIT_FAILURE);
        }
}
