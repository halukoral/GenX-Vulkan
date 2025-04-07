
cmake_minimum_required(VERSION 3.15)
project(GenX)

set(CMAKE_CXX_STANDARD 17)

# Set build types
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Vulkan SDK: support both ENV and -D
if(NOT DEFINED VULKAN_SDK AND NOT DEFINED ENV{VULKAN_SDK})
    message(FATAL_ERROR "VULKAN_SDK is not set. Please pass -DVULKAN_SDK=... or set environment variable.")
endif()

if(NOT DEFINED VULKAN_SDK)
    set(VULKAN_SDK $ENV{VULKAN_SDK})
endif()

set(VULKAN_SDK_INCLUDE "${VULKAN_SDK}/Include")
set(VULKAN_SDK_LIB "${VULKAN_SDK}/Lib")

# Output directories (set to project root)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(Engine)
add_subdirectory(Editor)

# Set startup project for Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Editor)
