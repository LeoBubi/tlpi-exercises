/* 
Implement tee using I/O system calls. Implement the –a command-line 
option, which causes tee to append text to the end of a file if it 
already exists.
*/

#define _XOPEN_SOURCE 1

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define TEA_BUFSIZ 512

#define exitUsage(progName) { \
    printf("Usage: %s [-a] [FILE]...\n", progName); \
    exit(EXIT_FAILURE); \
}

#define exitErr(fun) { \
    perror(fun); \
    exit(EXIT_FAILURE); \
}


int main(int argc, char **argv)
{
    int fds[argc], fds_size = 1;
    fds[0] = STDOUT_FILENO;

    int append = 0;

    for (int opt; (opt = getopt(argc, argv, "a")) != -1; )
        switch (opt) 
        {
            case 'a':
                append = O_APPEND;
                break;
            default:
                exitUsage(argv[0]);
                break;
        }
    
    for (int i = optind; i < argc; i++) {
        int fd;
        fd = open(argv[i], O_WRONLY | O_CREAT | append, 0664);
        if (fd == -1)
            exitErr("open");
        fds[fds_size++] = fd;
    }

    char buffer[TEA_BUFSIZ];
    ssize_t rb;
    while((rb = read(STDIN_FILENO, buffer, TEA_BUFSIZ)) > 0)
        for (int i = 0; i < fds_size; i++)
            if (write(fds[i], buffer, rb) == -1)
                exitErr("write");

    if (rb == -1)
        exitErr("read");
    
    for (int i = 1; i < fds_size; i++)
        if (close(fds[i]) == -1)
            exitErr("close");

    exit(EXIT_SUCCESS);
}

