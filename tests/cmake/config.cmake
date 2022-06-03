################################################################################
#####
##### Tectiform TFLinux CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

function(build_and_run_test BINARY_NAME TEST_NAME SOURCES)
    add_executable(${BINARY_NAME} ${SOURCES})
    if(TEST_COMPILE_DEFINITIONS)
        target_compile_definitions(${BINARY_NAME} PUBLIC ${TEST_COMPILE_DEFINITIONS})
    endif()
    if(TEST_COMPILE_FLAGS)
        target_compile_options(${BINARY_NAME} PUBLIC ${TEST_COMPILE_FLAGS})
    endif()
    if(TEST_LINK_FLAGS)
        set_target_properties(${BINARY_NAME} PROPERTIES LINK_FLAGS "${TEST_LINK_FLAGS}")
    endif()
    target_link_libraries(${BINARY_NAME} PRIVATE CONAN_PKG::gtest ${STATIC_LIBRARY_NAME})
    if(TEST_LIBRARIES)
        target_link_libraries(${BINARY_NAME} PRIVATE ${TEST_LIBRARIES})
    endif()
    add_test(NAME ${TEST_NAME} COMMAND ${BINARY_NAME})
endfunction()

