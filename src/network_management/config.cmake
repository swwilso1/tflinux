################################################################################
#####
##### Tectiform TFLinux CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

include(FindJava)

find_package(Java 1.8 REQUIRED)

set(ANTLR_JAR antlr-4.11.1-complete.jar)
find_file(ANTLR_JAR_LOCATION ${ANTLR_JAR} PATHS "/usr/local/lib" "/usr/lib")

set(GENERATED_DHCPCD_SOURCE_FILES
        ${GENERATED_SOURCES_DIR}/dhcpcdBaseVisitor.cpp
        ${GENERATED_SOURCES_DIR}/dhcpcdBaseVisitor.h
        ${GENERATED_SOURCES_DIR}/dhcpcdLexer.cpp
        ${GENERATED_SOURCES_DIR}/dhcpcdLexer.h
        ${GENERATED_SOURCES_DIR}/dhcpcdParser.cpp
        ${GENERATED_SOURCES_DIR}/dhcpcdParser.h
        ${GENERATED_SOURCES_DIR}/dhcpcdVisitor.cpp
        ${GENERATED_SOURCES_DIR}/dhcpcdVisitor.h
        )

set_source_files_properties(
        ${GENERATED_DHCPCD_SOURCE_FILES}
        PROPERTIES GENERATED ON
        COMPILE_FLAGS -Wno-unused-parameter
)

add_custom_command(OUTPUT ${GENERATED_DHCPCD_SOURCE_FILES}
        COMMAND ${Java_JAVA_EXECUTABLE} -jar ${ANTLR_JAR_LOCATION} -o ${GENERATED_SOURCES_DIR} -no-listener -visitor -package "TF::Linux::dhcpcd" -Dlanguage=Cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/network_management/dhcpcd.g4
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/network_management/dhcpcd.g4 ${CMAKE_CURRENT_SOURCE_DIR}/src/network_management/config.cmake
        WORKING_DIRECTORY ${GENERATED_SOURCES_DIR}
        )

list(APPEND LIBRARY_HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/network_management/tfnetworkconfiguration.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/network_management/tfnetworkmanager.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/network_management/tfsystemnetworkinterface.hpp")

list(APPEND LIBRARY_SOURCE_FILES
        src/network_management/tfdhcpcd.hpp
        src/network_management/tfdhcpcdconfiguration.cpp
        src/network_management/tfdhcpcdconfiguration.hpp
        src/network_management/tfdhcpcdvisitor.cpp
        src/network_management/tfdhcpcdvisitor.hpp
        src/network_management/tfdnsmasqconfiguration.cpp
        src/network_management/tfdnsmasqconfiguration.hpp
        src/network_management/tfhostapdconfiguration.cpp
        src/network_management/tfhostapdconfiguration.hpp
        src/network_management/tfkeyvalueconfigfile.cpp
        src/network_management/tfkeyvalueconfigfile.hpp
        src/network_management/tfnetworkconfiguration.cpp
        src/network_management/tfnetworkconfiguration.hpp
        src/network_management/tfnetplan.cpp
        src/network_management/tfnetplan.hpp
        src/network_management/tfnetworkmanager.cpp
        src/network_management/tfnetworkmanager.hpp
        src/network_management/tfserviceconfiguration.hpp
        src/network_management/tfsystemnetworkinterface.cpp
        src/network_management/tfsystemnetworkinterface.hpp
        src/network_management/tfyamlinterface.cpp
        src/network_management/tfyamlinterface.hpp
        ${GENERATED_DHCPCD_SOURCE_FILES}
        )
