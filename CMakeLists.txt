cmake_minimum_required(VERSION 3.5)
project(arser LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(EXPORT_DIR "${CMAKE_CURRENT_BINARY_DIR}/cmake")
set(CMAKE_CXX_VERSION 23)

set(arser_CompilerOptions ${arser_CompilerOptions}
    -Werror
    -Wall
    -Wextra
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wnull-dereference
    -Wdouble-promotion
    -Wimplicit-fallthrough
)

include(static_analyzers)
include(GNUInstallDirs)

#
# find_package(package CONFIG REQUIRED)
# ...
#
# set(arser_ExternalLibraries package ...)
#

enable_testing()

add_subdirectory(arser)
add_subdirectory(tests)

