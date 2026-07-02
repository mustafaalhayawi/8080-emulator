#include <string>
#include <vector>
#include <stdint.h>

#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

template <typename T>
std::string int_to_hex(const T val);

void disassemble_8080_op(const std::vector<uint8_t>& inst, uint16_t& pc);

void disassemble_file(const std::string& filename);

#endif
