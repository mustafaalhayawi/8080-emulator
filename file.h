#pragma once

#include <fstream>
#include <cstdint>
#include <string>

int get_file_size(const std::string file_name);

bool load_rom(const std::string& file_name, uint8_t* memory, int file_size=0);

