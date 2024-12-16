cmake_minimum_required(VERSION 3.10)
project(TLPI_EXERCISES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set strict compilation flags
set(STRICT_FLAGS
    -Wall                    # Enable all standard warnings
    -Wextra                  # Enable extra warnings
    -Werror                  # Treat warnings as errors
    -pedantic                # Strict ISO C compliance
    -pedantic-errors         # Treat pedantic warnings as errors
    -fstack-protector-strong # Stack protection
    -D_FORTIFY_SOURCE=2      # Additional security checks
    -fno-strict-aliasing     # Prevent type-punning issues
    -Wformat=2               # Strict format string checks
    -Wformat-security        # Security issues in format strings
    -Wshadow                 # Shadow variable warnings
    -Wcast-qual              # Casting away const warnings
    -Wmissing-declarations   # Missing declaration warnings
    -Wstrict-prototypes      # Function declaration warnings
)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(lbcommon STATIC
    src/lib/error.c
    src/lib/generic.c
)
target_compile_options(lbcommon PRIVATE ${STRICT_FLAGS})

add_executable(tee src/ch04/tee.c)
target_compile_options(tee PRIVATE ${STRICT_FLAGS})
target_link_libraries(tee PRIVATE lbcommon)