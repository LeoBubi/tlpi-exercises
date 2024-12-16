#ifndef _ERROR_H
# define _ERROR_H

#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>

extern bool debug;
#define DEBUG_OPT  {"debug", no_argument, NULL, INT_MAX-'d'}
#define DEBUG_OPT_CASE \
    case (INT_MAX-'d'): \
        debug = true; \
        break

// terminate program execution with error
void error_exit(const char *format, ...);


#endif /* _ERROR_H */
