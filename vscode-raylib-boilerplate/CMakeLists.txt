cmake_minimum_required(VERSION 3.11) # Minimum for FetchContent

# Project name
set(PROJECT_NAME "RaylibApp")
project(${PROJECT_NAME} LANGUAGES CXX)

# C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -s")

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# macOS Release configuration (set before target creation)
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "13.3")
    
    # Universal binary for Release builds
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
        message(STATUS "Building universal binary (arm64 + x86_64)")
    endif()
endif()

# Raylib dependency - hybrid approach
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET)

# If raylib not found, fetch and build it
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
    endif()
endif()

# Executable
add_executable(${PROJECT_NAME} src/main.cpp)

# macOS App Bundle configuration
if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/Info.plist.in"
    )
endif()

# Link raylib
target_link_libraries(${PROJECT_NAME} raylib)

# macOS frameworks (required)
target_link_libraries(${PROJECT_NAME} 
    "-framework IOKit"
    "-framework Cocoa" 
    "-framework OpenGL")

# Release build optimizations
if(APPLE AND CMAKE_BUILD_TYPE STREQUAL "Release")
    # Strip symbols and dead code for smaller executable
    target_link_options(${PROJECT_NAME} PRIVATE 
        "-Wl,-dead_strip"
        "-Wl,-x"
    )
endif()