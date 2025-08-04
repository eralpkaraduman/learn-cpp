# AddPlaydateApplication function supporting both simulator and device builds
# Based on playdate-cpp with support for ARM device builds

function(add_playdate_application PLAYDATE_GAME_NAME)
    message(STATUS "Adding playdate application ${PLAYDATE_GAME_NAME}")

    if (TOOLCHAIN STREQUAL "armgcc")
        # Device build - create executable for ARM Cortex-M7
        message(STATUS "Building for Playdate device (ARM)")
        add_executable(${PLAYDATE_GAME_NAME})
        
        set_property(TARGET ${PLAYDATE_GAME_NAME} PROPERTY OUTPUT_NAME "${PLAYDATE_GAME_NAME}.elf")
        
        # Create Source directory for PDX
        add_custom_command(
            TARGET ${PLAYDATE_GAME_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/Source
        )
        
        # Copy pdxinfo to Source
        add_custom_command(
            TARGET ${PLAYDATE_GAME_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/src/pdxinfo ${CMAKE_CURRENT_BINARY_DIR}/Source/pdxinfo
        )
        
        # Copy and strip the ELF file to Source
        add_custom_command(
            TARGET ${PLAYDATE_GAME_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${PLAYDATE_GAME_NAME}.elf ${CMAKE_CURRENT_BINARY_DIR}/Source/pdex.elf
            COMMAND ${CMAKE_STRIP} --strip-unneeded -R .comment -g ${CMAKE_CURRENT_BINARY_DIR}/Source/pdex.elf
        )
        
        # Create .pdx file from Source directory
        add_custom_command(
            TARGET ${PLAYDATE_GAME_NAME} POST_BUILD
            COMMAND ${PDC} ${CMAKE_CURRENT_BINARY_DIR}/Source ${CMAKE_CURRENT_BINARY_DIR}/${PLAYDATE_GAME_NAME}.pdx
        )
        
    else()
        # Simulator build - create shared library for macOS
        message(STATUS "Building for Playdate simulator (macOS)")
        add_library(${PLAYDATE_GAME_NAME} SHARED)
        
        # Create Source directory for PDX
        add_custom_command(
            TARGET ${PLAYDATE_GAME_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/Source
        )
        
        # Copy pdxinfo to Source
        add_custom_command(
            TARGET ${PLAYDATE_GAME_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/src/pdxinfo ${CMAKE_CURRENT_BINARY_DIR}/Source/pdxinfo
        )

        # Copy built library to Source
        add_custom_command(
            TARGET ${PLAYDATE_GAME_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/lib${PLAYDATE_GAME_NAME}.dylib ${CMAKE_CURRENT_BINARY_DIR}/Source/pdex.dylib
        )

        # Create .pdx file from Source directory
        add_custom_command(
            TARGET ${PLAYDATE_GAME_NAME} POST_BUILD
            COMMAND ${PDC} ${CMAKE_CURRENT_BINARY_DIR}/Source ${CMAKE_CURRENT_BINARY_DIR}/${PLAYDATE_GAME_NAME}.pdx
        )
    endif()

    # Link with our core libraries
    target_link_libraries(${PLAYDATE_GAME_NAME} PUBLIC pdcpp_core)
endfunction()