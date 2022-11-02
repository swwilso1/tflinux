################################################################################
#####
##### Tectiform TFLinux CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

set(CMAKE_CXX_STANDARD 20)

set(GENERATED_SOURCES_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated_sources" CACHE STRING "Name of generated source directory")

include(cmake/platform_tests.cmake)

include(cmake/conan.cmake)

option(CONAN_BUILD_ALL "Require conan install to rebuild from source packages" OFF)

list(APPEND CONAN_REQUIRES gtest/1.11.0)
if (CONAN_BUILD_ALL)
    set(CONAN_BUILD_ARG all)
else()
    set(CONAN_BUILD_ARG missing)
endif()

conan_cmake_configure(
        REQUIRES ${CONAN_REQUIRES}
        GENERATORS cmake_multi
        OPTIONS ${CONAN_OPTIONS}
)

macro(DO_CONAN_INSTALL_PER_CONFIG_TYPE TYPE)
    conan_cmake_autodetect(CONAN_SETTINGS BUILD_TYPE ${TYPE})

    conan_cmake_install(
            PATH_OR_REFERENCE .
            BUILD ${CONAN_BUILD_ARG}
            REMOTE conancenter
            SETTINGS ${CONAN_SETTINGS}
    )
endmacro()

if (CMAKE_CONFIGURATION_TYPES)
    foreach(TYPE ${CMAKE_CONFIGURATION_TYPES})
        DO_CONAN_INSTALL_PER_CONFIG_TYPE(${TYPE})
    endforeach()
elseif(CMAKE_BUILD_TYPE)
    DO_CONAN_INSTALL_PER_CONFIG_TYPE(${CMAKE_BUILD_TYPE})
else()
    message(FATAL_ERROR "CMAKE_BUILD_TYPE required in order to build Conan dependencies")
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
include(${CMAKE_BINARY_DIR}/conanbuildinfo_debug.cmake OPTIONAL)
include(${CMAKE_BINARY_DIR}/conanbuildinfo_release.cmake OPTIONAL)
include(${CMAKE_BINARY_DIR}/conanbuildinfo_relwithdebinfo.cmake OPTIONAL)
include(${CMAKE_BINARY_DIR}/conanbuildinfo_minsizerel.cmake OPTIONAL)

conan_basic_setup(TARGETS)

list(APPEND CMAKE_PREFIX_PATH ${TFFOUNDATION_DIR})
find_package(TFFoundation)

include(cmake/headers.cmake)

set(GENERATED_SOURCES_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated_sources" CACHE STRING "Name of generated source directory")
set(LIBRARY_NAME "TFLinux" CACHE STRING "Library name")
set(SHARED_LIBRARY_NAME ${LIBRARY_NAME}-shared CACHE STRING "Name of shared library target")
set(STATIC_LIBRARY_NAME ${LIBRARY_NAME}-static CACHE STRING "Name of static library target")

set(CPACK_GENERATOR TXZ)
include(CPack)
