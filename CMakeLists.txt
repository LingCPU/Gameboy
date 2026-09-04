cmake_minimum_required(VERSION 3.4)

project(gameboy)

add_definitions(-std=c++20)
add_definitions(-stdlib=libc++)
add_definitions(-g)
add_definitions(-Wall -Wextra -Wpedantic)
add_definitions(-Wno-gnu-zero-variadic-macro-arguments)
add_definitions(-ferror-limit=10000)
add_definitions(-Wno-unused-parameter)
add_definitions(-Wno-unused-private-field)
add_definitions(-Wno-unused-function)

add_executable(emulator
    src/address.cpp
    src/clock.cpp
    src/cpp.cpp
    src/files.cpp
    src/gameboy.cpp
    src/main.cpp
    src/memory.cpp
    src/mmu.cpp
    src/opcodeLink.cpp
    src/opcodeLinkCB.cpp
    src/opcodes.cpp
    src/register.cpp
)