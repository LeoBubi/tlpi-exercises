#include <lb/generic.h>

// program info
static proginfo_t program_info = {"", "", "", ""};

void __set_program_info(
    const char *name,
    const char *version,
    const char *release_date,
    const char *authors) 
{
    if (name) program_info.name = name;
    if (version) program_info.version = version;
    if (release_date) program_info.release_date = release_date;
    if (authors) program_info.authors = authors;
}

const proginfo_t* get_program_info(void) {
    return &program_info;
}
