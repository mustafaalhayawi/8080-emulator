#pragma once

#include "file.h"
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>
#include <iomanip>

void disassemble_8080_op(uint8_t* memory, uint16_t& pc);

void disassemble_file(const std::string& filename);

