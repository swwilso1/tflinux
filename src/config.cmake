################################################################################
#####
##### Tectiform TFLinux CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

include(src/exceptions/config.cmake)
include(src/files/config.cmake)
include(src/include/config.cmake)
include(src/mounttable/config.cmake)
include(src/udev/config.cmake)

include_directories(
    src/exceptions
    src/files
    src/include
    src/mounttable
    src/udev
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/headers/${LIBRARY_NAME})

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/headers/${LIBRARY_NAME}/updated
        COMMAND cp ${LIBRARY_HEADER_FILES} ${CMAKE_CURRENT_BINARY_DIR}/headers/${LIBRARY_NAME}
        COMMAND touch ${CMAKE_CURRENT_BINARY_DIR}/headers/${LIBRARY_NAME}/updated
        DEPENDS ${HEADER_FILES}
        COMMENT "Collecting ${LIBRARY_NAME} headers"
        )

add_custom_target(LinuxHeaders DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/headers/${LIBRARY_NAME}/updated)

list(APPEND COMPILE_OPTIONS -Wall -Wextra -Wconversion -Wsign-conversion)
#list(APPEND COMPILE_FLAGS -Werror -pedantic-errors)

if(BUILD_PROFILE)
    list(APPEND COMPILE_OPTIONS -pg)
endif()

if (BUILD_SANITIZER)
    list(APPEND COMPILE_OPTIONS -fsanitize=address -fno-omit-frame-pointer)
endif()

add_library(${SHARED_LIBRARY_NAME} SHARED ${LIBRARY_SOURCE_FILES} ${LIBRARY_HEADER_FILES})
set_target_properties(${SHARED_LIBRARY_NAME} PROPERTIES
    OUTPUT_NAME ${LIBRARY_NAME})
target_compile_options(${SHARED_LIBRARY_NAME} PRIVATE ${COMPILE_OPTIONS})
target_include_directories(${SHARED_LIBRARY_NAME} INTERFACE
        $<INSTALL_INTERFACE:include/TFLinux>)
target_link_libraries(${SHARED_LIBRARY_NAME} PRIVATE
        TFFoundation::TFFoundation-shared
        udev)
target_link_libraries(${SHARED_LIBRARY_NAME} INTERFACE
        TFFoundation::TFFoundation-shared
        udev)
add_dependencies(${SHARED_LIBRARY_NAME} LinuxHeaders)


add_library(${STATIC_LIBRARY_NAME} STATIC ${LIBRARY_SOURCE_FILES} ${LIBRARY_HEADER_FILES})
set_target_properties(${STATIC_LIBRARY_NAME} PROPERTIES
        OUTPUT_NAME ${LIBRARY_NAME})
target_compile_options(${STATIC_LIBRARY_NAME} PRIVATE ${COMPILE_OPTIONS})
target_include_directories(${STATIC_LIBRARY_NAME} INTERFACE
        $<INSTALL_INTERFACE:include/TFLinux>)
target_link_libraries(${STATIC_LIBRARY_NAME} PRIVATE
        TFFoundation::TFFoundation-static
        udev)
target_link_libraries(${STATIC_LIBRARY_NAME} INTERFACE
        TFFoundation::TFFoundation-static
        udev)
add_dependencies(${STATIC_LIBRARY_NAME} LinuxHeaders)

install(TARGETS ${SHARED_LIBRARY_NAME}
        ${STATIC_LIBRARY_NAME}
        EXPORT ${LIBRARY_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )

install(FILES ${LIBRARY_HEADER_FILES} DESTINATION
        include/${LIBRARY_NAME}
        )

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}/${LIBRARY_NAME}ConfigVersion.cmake"
        VERSION ${TFLinux_VERSION}
        COMPATIBILITY AnyNewerVersion
)

export(EXPORT ${LIBRARY_NAME}
        FILE "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}/${LIBRARY_NAME}Targets.cmake"
        NAMESPACE ${LIBRARY_NAME}::
        )

configure_file(cmake/${LIBRARY_NAME}Config.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}/${LIBRARY_NAME}Config.cmake"
        COPYONLY)

set(ConfigPackageLocation lib/cmake/${LIBRARY_NAME})
install(EXPORT ${LIBRARY_NAME}
        FILE ${LIBRARY_NAME}Targets.cmake
        NAMESPACE ${LIBRARY_NAME}::
        DESTINATION ${ConfigPackageLocation})
install(FILES
        cmake/${LIBRARY_NAME}Config.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}/${LIBRARY_NAME}ConfigVersion.cmake"
        DESTINATION ${ConfigPackageLocation})
