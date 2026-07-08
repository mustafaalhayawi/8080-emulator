#include "disassembler.h"
#include "cpu.h"
#include "file.h"
#include <iostream>

int main() {
    State state;

    state.memory = new uint8_t[65536]();

    disassemble_file("8080PRE.COM");
    
    load_rom("8080PRE.COM", state.memory + 0x0100);
    state.pc = 0x0100;
    state.sp = 0xFFFF;

    bool trace = false;
    while (true) {
        if (state.pc == 0x0689) {
            trace = true;
            uint16_t err_addr = (state.memory[state.sp+1] << 8) | state.memory[state.sp];
            std::cout << "\nFatal error at address: " << int_to_hex(err_addr) << "\n";
        }

        if (state.pc == 0x0000) {
            break;
        } else if (state.pc == 0x0005) {
            if (state.c == 0x09) {
                char out_char = state.memory[state.de];
                int i = 0;
                while (out_char != '$') {
                    std::cout << (char)out_char << std::flush;
                    i++;
                    out_char = state.memory[state.de+i];
                }
            } else if (state.c == 0x02){
                std::cout << (char)state.e << std::flush;
            }
            state.pc = (state.memory[state.sp+1] << 8) | (state.memory[state.sp]);
            state.sp += 2;
        } else {
            step_cpu(state);
        }
        if (trace) std::cout << "pc: " << int_to_hex(state.pc) << "\n";
    }

    delete[] state.memory;

    return 0;
}

