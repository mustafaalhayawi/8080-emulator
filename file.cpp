#include "file.h"

int get_file_size(const std::string file_name) {
    std::ifstream infile;
    infile.open(file_name, std::ios::binary | std::ios::in);
    infile.seekg(0, std::ios::end);
    int file_size = infile.tellg();
    infile.close();

    return file_size;
}

bool load_rom(const std::string& file_name, uint8_t* memory, int file_size) {
    std::ifstream infile;
    if (file_size == 0) file_size = get_file_size(file_name);
    infile.open(file_name, std::ios::binary | std::ios::in);
    infile.read(reinterpret_cast<char*>(memory), file_size);
    infile.close();

    return file_size > 0;
}

