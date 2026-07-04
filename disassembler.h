#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <sstream>
#include <iomanip>

template <typename T>
std::string int_to_hex(const T val) {
    std::stringstream stream;
    stream << "0x" << std::setfill('0') << std::setw(sizeof(T)*2) << std::hex << static_cast<unsigned int>(val);
    return stream.str();
}

void disassemble_8080_op(const std::vector<uint8_t>& inst, uint16_t& pc);

void disassemble_file(const std::string& filename);

