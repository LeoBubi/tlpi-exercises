/* 
Implement tee using I/O system calls. Implement the –a command-line 
option, which causes tee to append text to the end of a file if it 
already exists.
*/

#define PROGRAM_NAME "tee"
#define VERSION      "1.0"
#define RELEASE_DATE "2024-12-16"
#define AUTHORS      "Leonardo Brugnano"

#include <lb/error.h>
#include <lb/generic.h>

#define HELP_INFO   "Usage: "PROGRAM_NAME" [OPTION]... [FILE]...\n" \
                    "Copy standard input to each FILE, and also to standard output.\n" \
                    "\n" \
                    "\n" \
                    "  -a, --append     append to the given FILEs, do not overwrite\n" \
                    "      --debug      print additional details for errors\n"

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


int main(int argc, char **argv)
{
    set_program_info();
    
    /*********************/

    int oflag = O_TRUNC;

    int fds[argc-1];
    int fds_size;

    char buffer[READ_SIZE];
    ssize_t rb;

    /* read command-line options */

    const char optstring[] = "a";
    const struct option longopts[] = {
        {"append", no_argument, NULL, 'a'},
        DEBUG_OPT,
        DEFAULT_LAST_OPTS
    };

    int optchar;
    while ((optchar = getopt_long(argc, argv, optstring, longopts, NULL)) != -1)
        switch (optchar) {
            case 'a':
                oflag = O_APPEND;
                break;
            DEBUG_OPT_CASE;
            DEFAULT_LAST_OPTS_CASES;
        }
    
    /* open files for writing */
    
    fds[0] = STDOUT_FILENO;
    fds_size = 1;
    
    for (int i = optind; i < argc; i++, fds_size++)
        if ((fds[fds_size] = open(argv[i], O_WRONLY | O_CREAT | oflag, 0640)) == -1)
            error_exit("Unable to open file '%s' for writing", argv[i]);

    /* start copying */

    loop {
        if ((rb = read(STDIN_FILENO, buffer, READ_SIZE)) == -1)
            error_exit("Error reading standard input");
        
        if (!rb) break;

        for (int i = 0; i < fds_size; i++) {
            if (write(fds[i], buffer, rb) == -1)
                error_exit("Error writing to file");
        }
    }

    for (int i = 1; i < fds_size; i++) {
        if (close(fds[i]) == -1 && debug)
            perror(PROGRAM_NAME": close");
    }
}
