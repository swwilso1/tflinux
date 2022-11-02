################################################################################
#####
##### Tectiform TFLinux CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

file(MAKE_DIRECTORY ${GENERATED_SOURCES_DIR})

list(APPEND LIBRARY_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/include/TFLinux.hpp")

configure_file("src/include/tfconfigure.hpp.in"
        "${GENERATED_SOURCES_DIR}/tfconfigure.hpp")
