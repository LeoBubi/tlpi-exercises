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
#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define READ_SIZE 1024

#ifdef loop
#  undef loop
#endif
#define loop while (true)

bool verbose = false;

#define help() do { \
    fprintf(stdout, \
        "Usage: "PROGRAM_NAME" [OPTION]... [FILE]...\n" \
        "Copy standard input to each FILE, and also to standard output.\n" \
        "\n" \
        "  -a, --append     append to the given FILEs, do not overwrite\n" \
        "  -v, --verbose    be verbose with errors\n" \
        "\n" \
        "    --help      display help and exit\n" \
        "    --version   output version information and exit\n" \
        "\n"); \
    exit(EXIT_SUCCESS); \
} while (0)

#define version() do { \
    fprintf(stdout, \
        PROGRAM_NAME" version "VERSION" ("RELEASE_DATE")\n" \
        "Written by "AUTHORS"\n"); \
    exit(EXIT_SUCCESS); \
} while(0)

void error_exit(const char *format, ...) 
{
    fprintf(stderr, PROGRAM_NAME": ");
    
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    fprintf(stderr, ".\n");
    if (verbose) perror(NULL);
    exit(EXIT_FAILURE);
}


int main(int argc, char **argv)
{
    int oflag = O_TRUNC;

    int fds[argc-1];
    int fds_size;

    char buffer[READ_SIZE];
    ssize_t rb;

    /* read command-line options */

    const char optstring[] = "av";
    const struct option longopts[] = {
        {"append", no_argument, NULL, 'a'},
        {"verbose", no_argument, NULL, 'v'},
        {"help", no_argument, NULL, INT_MAX-'h'},
        {"version", no_argument, NULL, INT_MAX-'v'},
        {0, 0, 0, 0}
    };

    int optchar;
    while ((optchar = getopt_long(argc, argv, optstring, longopts, NULL)) != -1)
        switch (optchar) {
            case 'a':
                oflag = O_APPEND;
                break;
            case 'v':
                verbose = true;
                break;
            case (INT_MAX-'h'):
                help();
            case (INT_MAX-'v'):
                version();
            case '?':
                fprintf(stderr, 
                    PROGRAM_NAME": invalid option -- '%c'\n"
                    "Try '"PROGRAM_NAME" --help' for more information\n"
                    , (char)optopt);
                exit(EXIT_FAILURE);
            default:
                break;
        }
    
    /* open files for writing */
    
    fds[0] = STDOUT_FILENO;
    fds_size = 1;
    
    for (int i = optind; i < argc; i++, fds_size++)
        if ((fds[fds_size] = open(argv[i], O_WRONLY | O_CREAT | oflag, 0640)) == -1)
            error_exit("Unable to open file '%s' for writing", argv[i]);

    /* start copying */

    loop {
        if ((rb = read(STDOUT_FILENO, buffer, READ_SIZE)) == -1)
            error_exit("Error reading standard input");
        
        if (!rb) break;

        for (int i = 0; i < fds_size; i++) {
            if (write(fds[i], buffer, rb) == -1)
                error_exit("Error writing to file");
        }
    }

    for (int i = 1; i < fds_size; i++) {
        if (close(fds[i]) == -1 && verbose)
            perror(PROGRAM_NAME": close");
    }
}
