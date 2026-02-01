# CMakeLists.txt - EMIC ESP-IDF Project Template
# Minimum ESP-IDF version: 5.0

cmake_minimum_required(VERSION 3.16)

# Set the project components directory
set(EXTRA_COMPONENT_DIRS
    "${CMAKE_CURRENT_LIST_DIR}/components"
    "${CMAKE_CURRENT_LIST_DIR}/emic_sdk"
)

# Include ESP-IDF build system
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Project name (will be replaced by EMIC generator)
project(.{project.name}.)

# Add custom compile definitions
add_compile_definitions(
    EMIC_PROJECT=1
    EMIC_TARGET_ESP32=1
)

# Enable C11 and C++14 standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
