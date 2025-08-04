cmake_minimum_required(VERSION 3.19)

# Get project name from directory
get_filename_component(PLAYDATE_GAME_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PLAYDATE_GAME_NAME})

# C++ 20 for device compatibility
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_XCODE_GENERATE_SCHEME TRUE)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find Playdate SDK
set(ENVSDK $ENV{PLAYDATE_SDK_PATH})
file(TO_CMAKE_PATH ${ENVSDK} SDK)
message(STATUS "Playdate SDK Path: " ${SDK})
set(PDC "${SDK}/bin/pdc" -sdkpath "${SDK}" CACHE FILEPATH "path to the Playdate Compiler")

# Include our custom CMake functions
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/AddPlaydateApplication.cmake)

# Build the C API as a static library first
add_library(playdate_sdk STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/setup.c)

target_compile_definitions(playdate_sdk PUBLIC TARGET_EXTENSION=1)

if (TOOLCHAIN STREQUAL "armgcc")
    message(STATUS "Configuring for Playdate device (ARM)")
    
    # Device-only compile definitions and flags
    set(HEAP_SIZE 8388208)
    set(STACK_SIZE 61800)
    set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -x assembler-with-cpp -D__HEAP_SIZE=${HEAP_SIZE} -D__STACK_SIZE=${STACK_SIZE}")
    set(MCFLAGS -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -D__FPU_USED=1)

    target_compile_definitions(playdate_sdk PUBLIC TARGET_PLAYDATE=1)
    target_compile_options(playdate_sdk PUBLIC -Wall -Wno-unknown-pragmas -Wdouble-promotion)
    target_compile_options(playdate_sdk PRIVATE $<$<CONFIG:DEBUG>:-O2>)
    target_compile_options(playdate_sdk INTERFACE $<$<CONFIG:DEBUG>:-O0>)
    target_compile_options(playdate_sdk PUBLIC $<$<CONFIG:RELEASE>:-O3>)

    target_compile_options(playdate_sdk PUBLIC ${MCFLAGS})
    target_compile_options(playdate_sdk PUBLIC -falign-functions=16 -fomit-frame-pointer)
    target_compile_options(playdate_sdk PUBLIC -gdwarf-2)
    target_compile_options(playdate_sdk PUBLIC -fverbose-asm)
    target_compile_options(playdate_sdk PUBLIC -ffunction-sections -fdata-sections)
    target_compile_options(playdate_sdk PUBLIC -mword-relocations -fno-common)
    target_compile_options(playdate_sdk PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>)

    target_link_options(playdate_sdk PUBLIC ${MCFLAGS})
    target_link_options(playdate_sdk PUBLIC -T${SDK}/C_API/buildsupport/link_map.ld)
    target_link_options(playdate_sdk PUBLIC "-Wl,-Map=game.map,--cref,--gc-sections,--no-warn-mismatch,--emit-relocs")
    target_link_options(playdate_sdk PUBLIC --entry eventHandlerShim)
    target_link_options(playdate_sdk PUBLIC -static-libgcc -static-libstdc++)
else()
    message(STATUS "Configuring for Playdate simulator (macOS)")
    
    # Simulator build definitions
    target_compile_definitions(playdate_sdk PUBLIC TARGET_SIMULATOR=1)
    target_compile_options(playdate_sdk PUBLIC -Wall -Wno-unknown-pragmas -Wdouble-promotion)
    target_compile_options(playdate_sdk PUBLIC $<$<CONFIG:DEBUG>:-ggdb -O0>)
    target_compile_options(playdate_sdk PUBLIC $<$<CONFIG:RELEASE>:-O2>)
endif()

target_include_directories(playdate_sdk PUBLIC ${SDK}/C_API ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Build the C++ wrapper library
add_library(pdcpp_core STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/pdnewlib.c)
target_include_directories(pdcpp_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(pdcpp_core playdate_sdk)

# Create our game application
add_playdate_application(${PLAYDATE_GAME_NAME})

# Add our game sources
target_sources(${PLAYDATE_GAME_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

# Ensure debug info for our game target
target_compile_options(${PLAYDATE_GAME_NAME} PUBLIC $<$<CONFIG:DEBUG>:-ggdb -O0>)
target_compile_options(${PLAYDATE_GAME_NAME} PUBLIC $<$<CONFIG:RELEASE>:-O2>)

# Device builds need additional C++ flags
if (TOOLCHAIN STREQUAL "armgcc")
    target_compile_options(${PLAYDATE_GAME_NAME} PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions -fno-rtti>)
endif()