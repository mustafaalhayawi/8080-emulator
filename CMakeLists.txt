cmake_minimum_required(VERSION 3.23)

project(8080_emulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_executable(emulator main.cpp disassembler.cpp cpu.cpp file.cpp bus.cpp graphics.cpp driver.cpp)
target_link_libraries(emulator PRIVATE SDL3::SDL3)
