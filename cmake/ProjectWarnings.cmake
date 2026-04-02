# Compiler-specific warning flags (only for our code)
add_library(project_warnings INTERFACE)

if(MSVC)
    target_compile_options(project_warnings INTERFACE /W4 /WX /external:anglebrackets /external:W0)
else()
    target_compile_options(project_warnings INTERFACE -Wall -Wextra -Wpedantic -Werror)
endif()
