# Compiler-specific project flags (only for our code)
add_library(project_warnings INTERFACE)

if(MSVC)
    target_compile_options(project_warnings INTERFACE /W4 /WX /EHs-c- /external:anglebrackets /external:W0)
    target_compile_definitions(project_warnings INTERFACE _HAS_EXCEPTIONS=0)
else()
    target_compile_options(project_warnings INTERFACE -Wall -Wextra -Wpedantic -Werror -fno-exceptions)
endif()
