cmake_minimum_required(VERSION 3.23)

project(8080_emulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(emulator main.cpp disassembler.cpp)
