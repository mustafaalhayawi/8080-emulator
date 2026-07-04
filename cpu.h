#pragma once

#include "file.h"
#include <cstdint>
#include <fstream>
#include <iostream> // temp

enum ConditionFlags {
    FLAG_CY = 1 << 0, // carry - 0x01
    // skip bit 1
    FLAG_P = 1 << 2, // parity - 0x04
    // skip bit 3
    FLAG_AC = 1 << 4, // auxiliary carry - 0x10
    // skip bit 5
    FLAG_Z = 1 << 6, // zero - 0x40
    FLAG_S = 1 << 7 // sign - 0x80
};

struct State {
    uint8_t* memory;

    uint16_t pc = 0; // program counter
    uint16_t sp = 0; // stack pointer

    // general purpose registers
    uint8_t b = 0x00, c = 0x00;
    uint8_t d = 0x00, e = 0x00;
    uint8_t h = 0x00, l = 0x00;

    // ALU
    uint8_t a = 0x00; // accumulator
    uint8_t flags = 0x02;
};

uint8_t fetch_byte(State& state);
uint16_t fetch_word(State& state);

void step_cpu(State& state);

