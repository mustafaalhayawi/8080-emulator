#pragma once

#include "file.h"
#include "bus.h"
#include <cstdint>
#include <fstream>
#include <iostream> // temp

enum Flag {
    CY = 1 << 0, // carry - 0x01
    // 1
    P = 1 << 2, // parity - 0x04
    // 0 
    AC = 1 << 4, // auxiliary carry - 0x10
    // 0
    Z = 1 << 6, // zero - 0x40
    S = 1 << 7 // sign - 0x80
};

struct State {
    uint8_t* memory;

    uint16_t pc = 0; // program counter
    uint16_t sp = 0; // stack pointer

    // general purpose registers
    union {
        uint16_t bc;
        struct {
            uint8_t c = 0x00;
            uint8_t b = 0x00;
        };
    };

    union {
        uint16_t de;
        struct {
            uint8_t e = 0x00;
            uint8_t d = 0x00;
        };
    };

    union {
        uint16_t hl;
        struct {
            uint8_t l = 0x00;
            uint8_t h = 0x00;
        };
    };

    Bus bus;

    // ALU
    uint8_t a = 0x00; // accumulator
    uint8_t flags = 0x02;

    bool int_enabled = false;
    int ei_delay = 0;
    bool halted = false;
    int total_states = 0;
};

void set_flag(State& state, Flag flag, bool cond);
bool get_flag(State& state, Flag flag);

void inst_add(State& state, uint8_t val, bool carry=false);
void inst_sub(State& state, uint8_t val, bool carry=false);
void inst_inr(State& state, uint8_t& num);
void inst_dcr(State& state, uint8_t& num);
void inst_dad(State& state, uint16_t rp);
void inst_daa(State& state);
void inst_and(State& state, uint8_t val, bool clear_ac=false);
void inst_xor(State& state, uint8_t val);
void inst_or(State& state, uint8_t);
void inst_cmp(State& state, uint8_t val);
void inst_rlc(State& state);
void inst_rrc(State& state);
void inst_ral(State& state);
void inst_rar(State& state);
void inst_call(State& state, bool cond);
void inst_ret(State& state, bool cond);
void inst_rst(State& state, uint8_t n);

bool parity(uint8_t val);

uint8_t get_high_byte(uint16_t word);
uint8_t get_low_byte(uint16_t word); 

uint8_t fetch_byte(State& state);
uint16_t fetch_word(State& state);

void step_cpu(State& state);

