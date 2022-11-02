################################################################################
#####
##### Tectiform TFLinux CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

include(CheckSymbolExists)
include(CheckSourceCompiles)

if (UNIX)
    set(CMAKE_REQUIRED_LIBRARIES udev)
    try_compile(HAVE_UDEV_DEVICE_GET_CURRENT_TAGS_LIST_ENTRY ${CMAKE_CURRENT_BINARY_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/cmake/udev_test.cpp
            LINK_LIBRARIES udev
            CXX_STANDARD 20)
#    check_source_compiles(CXX
#            "#include <libudev.h> int main() {auto entry = udev_device_get_current_tags_list_entry(nullptr); return 0;}"
#            HAVE_UDEV_DEVICE_GET_CURRENT_TAGS_LIST_ENTRY)
#    check_symbol_exists(udev_device_get_current_tags_list_entry libudev.h HAVE_UDEV_DEVICE_GET_CURRENT_TAGS_LIST_ENTRY)
endif()
