cmake_minimum_required(VERSION 3.15) # Recommended for raylib

# Project name
set(PROJECT_NAME "RaylibApp")
project(${PROJECT_NAME} LANGUAGES C)

# C11 standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -s")

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
    message(STATUS "Raylib ${RAYLIB_VERSION} not found locally, downloading and building from source...")
    include(FetchContent)
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)
        message(STATUS "Fetching raylib ${RAYLIB_VERSION} source code...")
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
        message(STATUS "Successfully built raylib ${RAYLIB_VERSION} from source")
    endif()
else()
    message(STATUS "Found raylib ${RAYLIB_VERSION} installed on system, using pre-installed version")
endif()

# Executable
add_executable(${PROJECT_NAME} src/main.c)

# Add icon to app bundle
if(APPLE)
    set_source_files_properties(icon.icns PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )
    target_sources(${PROJECT_NAME} PRIVATE icon.icns)
endif()

# macOS App Bundle configuration (all builds)
if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/Info.plist.in"
    )
endif()

# Copy assets relative to binary for all platforms
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMENT "Copying assets relative to binary"
)

# Link raylib
target_link_libraries(${PROJECT_NAME} raylib)

# macOS frameworks (required)
if(APPLE)
    target_link_libraries(${PROJECT_NAME} 
        "-framework IOKit"
        "-framework Cocoa" 
        "-framework OpenGL")
endif()

# Release build optimizations
if(APPLE AND CMAKE_BUILD_TYPE STREQUAL "Release")
    # Strip symbols and dead code for smaller executable
    target_link_options(${PROJECT_NAME} PRIVATE 
        "-Wl,-dead_strip"
        "-Wl,-x"
    )
endif()