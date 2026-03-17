# Compiler-specific warning flags (only for our code)
if(MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror=all)
endif()


add_subdirectory(game)
