#include "disassembler.h"
#include "cpu.h"
#include "file.h"
#include <iostream>

int main() {
    State state;

    state.memory = new uint8_t[65536]();

    // load_rom("8080PRE.COM", state.memory);
    
    // test rom
    state.memory[0] = 0x3e; // MVI A, 0x42
    state.memory[1] = 0x42;
    state.memory[2] = 0x47; // MOV B, A
    state.memory[3] = 0x04; // INR B 
    state.memory[4] = 0xC3; // JMP 0x0004
    state.memory[5] = 0x04;
    state.memory[6] = 0x00;

    while (true) {
        step_cpu(state);
        std::cout << int_to_hex(state.a) << " " << int_to_hex(state.b) << "\n";
    }

    delete[] state.memory;

    return 0;
}

