################################################################################
#####
##### Tectiform TFLinux CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

cmake_minimum_required(VERSION 3.21)

project(TFLinux)

set(TFLinux_VERSION 0.1.0)

include(cmake/config.cmake)
include(src/config.cmake)

option(BUILD_TESTS "create library test programs" OFF)

if (BUILD_TESTS)
    enable_testing()
    include(tests/config.cmake)
endif()
