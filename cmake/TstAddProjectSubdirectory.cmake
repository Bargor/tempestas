# Adds a directory containing project-owned targets with the project's compiler policy.
# CMake enables /EHsc for MSVC by default, which conflicts with the /EHs-c- option
# applied by project_warnings. Remove the default only in this function's scope so
# project targets compile without exceptions while third-party targets keep their
# upstream exception settings.
function(tst_add_project_subdirectory directory)
    if(MSVC)
        string(REGEX REPLACE " ?/EH[a-zA-Z0-9+-]*" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    endif()

    add_subdirectory("${directory}")
endfunction()
