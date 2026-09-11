cmake_minimum_required(VERSION 3.16)

project(gameboy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(emulator
    src/address.cpp
    src/cartridge.cpp
    src/clock.cpp
    src/cpu.cpp
    src/files.cpp
    src/gameboy.cpp
    src/main.cpp
    src/mmu.cpp
    src/opcodeLink.cpp
    src/opcodeLinkCB.cpp
    src/opcodes.cpp
    src/register.cpp
)

target_include_directories(emulator PRIVATE include)

target_compile_options(emulator PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)