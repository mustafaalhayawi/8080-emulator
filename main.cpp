#include "disassembler.h"
#include "cpu.h"
#include "file.h"
#include <iostream>

int main() {
    State state;

    state.memory = new uint8_t[65536]();

    load_rom("8080PRE.COM", state.memory);

    disassemble_file("8080PRE.COM");

    delete[] state.memory;

    return 0;
}
