################################################################################
#####
##### Tectiform TFLinux CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND LIBRARY_HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/files/tfautofiledescriptor.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/files/tffileobserver.hpp")

list(APPEND LIBRARY_SOURCE_FILES
        src/files/tffileobserver.cpp)
