#pragma once

#include <fstream>
#include <sstream>
#include <cstdint>
#include <iomanip>
#include <string>

template <typename T>
std::string int_to_hex(const T val) {
    std::stringstream stream;
    stream << "0x" << std::setfill('0') << std::setw(sizeof(T)*2) << std::hex << static_cast<unsigned int>(val);
    return stream.str();
}

int get_file_size(const std::string file_name);

bool load_rom(const std::string& file_name, uint8_t* memory, int file_size=0);

