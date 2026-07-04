#include "cpu.h"

uint8_t fetch_byte(State& state) {
    uint8_t byte = state.memory[state.pc];
    state.pc++;

    return byte;
}

uint16_t fetch_word(State& state) {
    uint8_t low = state.memory[state.pc];
    uint8_t high = state.memory[state.pc+1];
    uint16_t word = low | (high << 8);
    state.pc += 2;

    return word;
}

void step_cpu(State& state) {
    uint8_t opcode = fetch_byte(state);
    //std::cout << state.pc << " " << int_to_hex(opcode) << "\n";

    switch (opcode) {
        case 0x00:
            break;
        case 0x3e:
            state.a = fetch_byte(state);
            break;
        case 0x47:
            state.b = state.a;
            break;
        case 0x04:
            state.b++;
            break;
        case 0xc3:
            state.pc = fetch_word(state);
            break;
    }
}

