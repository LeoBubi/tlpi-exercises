#ifndef _GENERIC_H
# define _GENERIC_H 1


// NULL
#ifdef NULL
# undef NULL
#endif
#define NULL ((void*)0)

// loop indefinitely
#ifdef loop
#  undef loop
#endif
#define loop while (true)

// program info
typedef struct proginfo {
    const char *name;
    const char *version;
    const char *release_date;
    const char *authors;
} proginfo_t;

/* NOT TO BE CALLED DIRECTLY */
void __set_program_info(
    const char *name,
    const char *version,
    const char *release_date,
    const char *authors
);
#define set_program_info() __set_program_info( \
                                        PROGRAM_NAME, \
                                        VERSION, \
                                        RELEASE_DATE, \
                                        AUTHORS )

const proginfo_t* get_program_info(void);
#define get_program_name()    get_program_info()->name
#define get_program_version() get_program_info()->version
#define get_program_release_date() get_program_info()->release_date
#define get_program_authors() get_program_info()->authors

// default longopts
#define DEFAULT_LAST_OPTS   {"help", no_argument, NULL, INT_MAX-'h'}, \
                            {"version", no_argument, NULL, INT_MAX-'v'}, \
                            {NULL, 0, NULL, 0}

#define DEFAULT_LAST_OPTS_CASES \
    case (INT_MAX-'h'): \
        help(); \
    case (INT_MAX-'v'): \
        version(); \
    case '?': \
        fprintf(stderr,  \
            PROGRAM_NAME": invalid option -- '%c'\n" \
            "Try '"PROGRAM_NAME" --help' for more information\n" \
            , (char)optopt); \
        exit(EXIT_FAILURE); \
    default: \
        break

// version information
#define version() do { \
    fprintf(stdout, \
        PROGRAM_NAME" version "VERSION" ("RELEASE_DATE")\n" \
        "Written by "AUTHORS"\n"); \
    exit(EXIT_SUCCESS); \
} while(0)

// help information
#define help() do { \
    fprintf(stdout, \
        HELP_INFO \
        "\n" \
        "    --help      display help and exit\n" \
        "    --version   output version information and exit\n" \
        "\n"); \
    exit(EXIT_SUCCESS); \
} while (0)


#endif /* _GENERIC_H */
