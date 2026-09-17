cmake_minimum_required(VERSION 3.16)

project(gameboy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(GAMEBOY_BUILD_SDL "Build the SDL3 frontend" ON)

add_library(gameboy_core
    src/address.cpp
    src/cartridge.cpp
    src/clock.cpp
    src/cpu.cpp
    src/files.cpp
    src/gameboy.cpp
    src/input.cpp
    src/mmu.cpp
    src/opcodeLink.cpp
    src/opcodeLinkCB.cpp
    src/opcodes.cpp
    src/ppu.cpp
    src/register.cpp
    src/serial.cpp
)

target_include_directories(gameboy_core PUBLIC include)

if(MSVC)
    target_compile_options(gameboy_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(gameboy_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(GAMEBOY_BUILD_SDL)
    find_package(SDL3 REQUIRED CONFIG)

    add_executable(emulator
        sdl/main.cpp
    )

    target_link_libraries(emulator PRIVATE gameboy_core SDL3::SDL3)

    if(MSVC)
        target_compile_options(emulator PRIVATE /W4 /permissive-)
    else()
        target_compile_options(emulator PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    if(WIN32 AND TARGET SDL3::SDL3-shared)
        add_custom_command(
            TARGET emulator POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    $<TARGET_FILE:SDL3::SDL3-shared>
                    $<TARGET_FILE_DIR:emulator>
            VERBATIM
        )
    endif()
endif()
