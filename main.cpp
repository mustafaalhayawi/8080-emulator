#include "disassembler.h"
#include "cpu.h"
#include "file.h"
#include "graphics.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    State state;

    /*state.memory = new uint8_t[65536]();

    //disassemble_file("8080EXM.COM");

    load_rom("8080EXM.COM", state.memory + 0x0100);
    state.pc = 0x0100;
    state.sp = 0xFFFF;

    bool trace = false;

    bool throttle_enabled = true;
    auto frame_start = std::chrono::high_resolution_clock::now();

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
                uint16_t ptr = state.de;
                while (ptr < 0x10000 && state.memory[ptr] != '$') {
                    std::cout << (char)state.memory[ptr] << std::flush;
                    ptr++;
                }
            } else if (state.c == 0x02){
                std::cout << (char)state.e << std::flush;
            }
            state.pc = (state.memory[state.sp+1] << 8) | (state.memory[state.sp]);
            state.sp += 2;
        } else {
            step_cpu(state);
        }

        if (trace) {
            std::cout << "opcode: " << int_to_hex(state.memory[state.pc]) << "\n";
            std::cout << "pc: " << int_to_hex(state.pc) << "\n";
            //std::cout << "sp: " << int_to_hex(state.sp) << "\n\n";
        }

        if (throttle_enabled && state.total_states >= 33333) {
            auto frame_end = std::chrono::high_resolution_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(frame_end - frame_start).count();

            if (elapsed_time < 16667) {
                std::this_thread::sleep_for(std::chrono::microseconds(16667 - elapsed_time));
            }

            state.total_states = 0;
            frame_start = std::chrono::high_resolution_clock::now();
        }
    }

    delete[] state.memory;*/

    int exit_code{ 0 };

    Renderer renderer;

    if (renderer.init() == false) {
        SDL_Log("Unable to initalise program!\n");
        exit_code = 1;
    } else {
        renderer.render();
    }

    renderer.close();

    return exit_code;
}

