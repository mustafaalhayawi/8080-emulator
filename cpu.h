#pragma once

#include <cstdint>

enum ConditionFlags {
    FLAG_CY = 1 << 0, // carry - 0x01
    // skip bit 1
    FLAG_P = 1 << 2, // parity - 0x04
    // skip bit 3
    FLAG_AC = 1 << 4, // auxiliary carry - 0x10
    // skipt bit 5
    FLAG_Z = 1 << 6, // zero - 0x40
    FLAG_S = 1 << 7 // sign - 0x80

};

struct State {
    uint8_t* memory;

    uint16_t pc; // program counter
    uint16_t sp; // stack pointer

    // general purpose registers
    uint8_t b, c;
    uint8_t d, e;
    uint8_t h, l;

    // ALU
    uint8_t a; // accumulator
    uint8_t flags = 0x02;
};

