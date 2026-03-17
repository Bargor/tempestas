# Check for CMake 4.2+ when using Visual Studio 2026
if(CMAKE_GENERATOR MATCHES "Visual Studio 18 2026")
    cmake_minimum_required(VERSION 4.2)
else()
    cmake_minimum_required(VERSION 3.30)
endif()

project(tempestas VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set build output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Project options
option(TST_ENABLE_TESTS "Enable building tests" ON)

# Add third-party libraries
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/glfw)

set(FMT_TEST OFF CACHE BOOL "" FORCE)
set(FMT_DOC OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/fmt)

# Main project configuration
set(PROJECT_SOURCES)

# Include subdirectories
add_subdirectory(src)

if(TST_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(third_party/googletest)
    # add_subdirectory(tests)
endif()
