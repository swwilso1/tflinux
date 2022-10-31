################################################################################
#####
##### Tectiform TFLinux CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND LIBRARY_HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/filesystems/tffilesystems.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/filesystems/tfmounter.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/filesystems/tfmounttable.hpp"
        )

list(APPEND LIBRARY_SOURCE_FILES
        src/filesystems/tffilesystems.cpp
        src/filesystems/tfmounter.cpp
        src/filesystems/tfmounttable.cpp
        )

