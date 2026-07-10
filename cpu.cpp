#include "cpu.h"

void set_flag(State& state, Flag flag, bool cond) {
    if (cond) {
        state.flags |= flag;
    } else {
        state.flags &= ~flag;
    }
}

bool get_flag(State& state, Flag flag) {
    return (state.flags & flag) != 0x00;
}

void inst_add(State& state, uint8_t val, bool carry) {
    uint16_t result = state.a + val + (carry ? get_flag(state, Flag::CY) : 0);

    set_flag(state, Flag::Z, (result & 0xFF) == 0);
    set_flag(state, Flag::S, (result & 0x80) != 0);
    set_flag(state, Flag::P, parity(get_low_byte(result)));
    set_flag(state, Flag::CY, result > get_low_byte(result));
    set_flag(state, Flag::AC, ((state.a ^ val ^ result) & 0x10) != 0);

    state.a = get_low_byte(result);
}

void inst_sub(State& state, uint8_t val, bool carry) {
    uint16_t result = state.a - val - (carry ? get_flag(state, Flag::CY) : 0);

    set_flag(state, Flag::Z, (result & 0xFF) == 0);
    set_flag(state, Flag::S, (result & 0x80) != 0);
    set_flag(state, Flag::P, parity(get_low_byte(result)));
    set_flag(state, Flag::CY, state.a < (val + (carry ? get_flag(state, Flag::CY) : 0)));
    set_flag(state, Flag::AC, ((state.a ^ ~val ^ result) & 0x10) != 0);

    state.a = get_low_byte(result);
}

void inst_inr(State& state, uint8_t& num) {
    uint16_t result = num + 1;

    set_flag(state, Flag::Z, (result & 0xFF) == 0);
    set_flag(state, Flag::S, (result & 0x80) != 0);
    set_flag(state, Flag::P, parity(get_low_byte(result)));
    set_flag(state, Flag::AC, ((num ^ 1 ^ result) & 0x10) != 0);

    num = get_low_byte(result);
}

void inst_dcr(State& state, uint8_t& num) {
    uint16_t result = num - 1;

    set_flag(state, Flag::Z, (result & 0xFF) == 0);
    set_flag(state, Flag::S, (result & 0x80) != 0);
    set_flag(state, Flag::P, parity(get_low_byte(result)));
    set_flag(state, Flag::AC, ((num ^ 0xFE ^ result) & 0x10) != 0);

    num = get_low_byte(result);
}

void inst_dad(State& state, uint16_t rp) {
    uint32_t result = state.hl + rp;
    set_flag(state, Flag::CY, result > static_cast<uint16_t>(result));
    state.hl = static_cast<uint16_t>(result);
}

void inst_daa(State& state) {
    uint8_t add = 0;
    bool cy = get_flag(state, Flag::CY);
    bool ac = get_flag(state, Flag::AC);

    if ((state.a & 0x0F) > 0x09 || ac) {
        add += 0x06;
    }

    if ((state.a >> 4) > 0x09 || cy || ((state.a >> 4) >= 0x09 && (state.a & 0x0F) > 0x09)) {
        add += 0x60;
        cy = true;
    }

    uint16_t result = state.a + add;

    set_flag(state, Flag::Z, get_low_byte(result) == 0);
    set_flag(state, Flag::S, (result & 0x80) != 0);
    set_flag(state, Flag::P, parity(get_low_byte(result)));
    set_flag(state, Flag::CY, cy);
    set_flag(state, Flag::AC, ((state.a ^ add ^ result) & 0x10) != 0);

    state.a = get_low_byte(result);
}

void inst_and(State& state, uint8_t val, bool clear_ac) {
    uint8_t result = state.a & val;

    set_flag(state, Flag::Z, result == 0);
    set_flag(state, Flag::S, (result & 0x80) != 0);
    set_flag(state, Flag::P, parity(result));
    set_flag(state, Flag::CY, false);
    set_flag(state, Flag::AC, ((state.a | val) & 0x08) != 0);

    state.a = result;
}

void inst_xor(State& state, uint8_t val) {
    uint8_t result = state.a ^ val;

    set_flag(state, Flag::Z, result == 0);
    set_flag(state, Flag::S, (result & 0x80) != 0);
    set_flag(state, Flag::P, parity(result));
    set_flag(state, Flag::CY, false);
    set_flag(state, Flag::AC, false);

    state.a = result;
}

void inst_or(State& state, uint8_t val) {
    uint8_t result = state.a | val;

    set_flag(state, Flag::Z, result == 0);
    set_flag(state, Flag::S, (result & 0x80) != 0);
    set_flag(state, Flag::P, parity(result));
    set_flag(state, Flag::CY, false);
    set_flag(state, Flag::AC, false);

    state.a = result;
}

void inst_cmp(State& state, uint8_t val) {
    uint16_t result = state.a - val;

    set_flag(state, Flag::Z, state.a == val);
    set_flag(state, Flag::S, (result & 0x80) != 0);
    set_flag(state, Flag::P, parity(get_low_byte(result)));
    set_flag(state, Flag::CY, state.a < val);
    set_flag(state, Flag::AC, ((state.a ^ ~val ^ result) & 0x10) != 0);
}

void inst_rlc(State& state) {
    uint8_t carry_bit = (state.a & 0x80) >> 7;
    set_flag(state, Flag::CY, carry_bit);
    state.a  = ((state.a << 1) & 0xFF) | carry_bit;
}

void inst_rrc(State& state) {
    uint8_t carry_bit = state.a & 0x01;
    set_flag(state, Flag::CY, carry_bit);
    state.a = (state.a >> 1) | (carry_bit << 7);
}

void inst_ral(State& state) {
    uint8_t carry_bit = (state.a & 0x80) >> 7;
    bool cy_flag = get_flag(state, Flag::CY);
    set_flag(state, Flag::CY, carry_bit == 0x01);
    state.a = ((state.a << 1) & 0xFF) | cy_flag;
}

void inst_rar(State& state) {
    uint8_t carry_bit = state.a & 0x01;
    bool cy_flag = get_flag(state, Flag::CY);
    set_flag(state, Flag::CY, carry_bit == 0x01);
    state.a = (state.a >> 1) | (cy_flag << 7);
}

void push_word(State& state, uint16_t val) {
    state.sp--;
    state.memory[state.sp] = get_high_byte(val);
    state.sp--;
    state.memory[state.sp] = get_low_byte(val);
}

uint16_t pop_word(State& state) {
    uint8_t low = state.memory[state.sp];
    state.sp++;
    uint8_t high = state.memory[state.sp];
    state.sp++;

    return (high << 8) | low;
}

void inst_call(State& state, bool cond) {
    uint16_t addr = fetch_word(state);

    if (!cond) return;

    push_word(state, state.pc);
    state.pc = addr;
}

void inst_ret(State& state, bool cond) {
    if (!cond) return;

    state.pc = pop_word(state);
}

void inst_rst(State& state, uint8_t n) {
    push_word(state, state.pc);
    state.pc = n << 3;
}


bool parity(uint8_t val) {
    val ^= val >> 4;
    val ^= val >> 2;
    val ^= val >> 1;

    return (val & 1) == 0;
}

uint8_t get_high_byte(uint16_t word) {
    return static_cast<uint8_t>(word >> 8);
}

uint8_t get_low_byte(uint16_t word) {
    return static_cast<uint8_t>(word);
}

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
        // MOV r1, r2
        case 0x40:
            state.b = state.b;
            break;
        case 0x41:
            state.b = state.c;
            break;
        case 0x42:
            state.b = state.d;
            break;
        case 0x43:
            state.b = state.e;
            break;
        case 0x44:
            state.b = state.h;
            break;
        case 0x45:
            state.b = state.l;
            break;
        case 0x47:
            state.b = state.a;
            break;
        case 0x48:
            state.c = state.b;
            break;
        case 0x49:
            state.c = state.c;
            break;
        case 0x4A:
            state.c = state.d;
            break;
        case 0x4B:
            state.c = state.e;
            break;
        case 0x4C:
            state.c = state.h;
            break;
        case 0x4D:
            state.c = state.l;
            break;
        case 0x4F:
            state.c = state.a;
            break;
        case 0x50:
            state.d = state.b;
            break;
        case 0x51:
            state.d = state.c;
            break;
        case 0x52:
            state.d = state.d;
            break;
        case 0x53:
            state.d = state.e;
            break;
        case 0x54:
            state.d = state.h;
            break;
        case 0x55:
            state.d = state.l;
            break;
        case 0x57:
            state.d = state.a;
            break;
        case 0x58:
            state.e = state.b;
            break;
        case 0x59:
            state.e = state.c;
            break;
        case 0x5A:
            state.e = state.d;
            break;
        case 0x5B:
            state.e = state.e;
            break;
        case 0x5C:
            state.e = state.h;
            break;
        case 0x5D:
            state.e = state.l;
            break;
        case 0x5F:
            state.e = state.a;
            break;
        case 0x60:
            state.h = state.b;
            break;
        case 0x61:
            state.h = state.c;
            break;
        case 0x62:
            state.h = state.d;
            break;
        case 0x63:
            state.h = state.e;
            break;
        case 0x64:
            state.h = state.h;
            break;
        case 0x65:
            state.h = state.l;
            break;
        case 0x67:
            state.h = state.a;
            break;
        case 0x68:
            state.l = state.b;
            break;
        case 0x69:
            state.l = state.c;
            break;
        case 0x6A:
            state.l = state.d;
            break;
        case 0x6B:
            state.l = state.e;
            break;
        case 0x6C:
            state.l = state.h;
            break;
        case 0x6D:
            state.l = state.l;
            break;
        case 0x6F:
            state.l = state.a;
            break;
        case 0x78:
            state.a = state.b;
            break;
        case 0x79:
            state.a = state.c;
            break;
        case 0x7A:
            state.a = state.d;
            break;
        case 0x7B:
            state.a = state.e;
            break;
        case 0x7C:
            state.a = state.h;
            break;
        case 0x7D:
            state.a = state.l;
            break;
        case 0x7F:
            state.a = state.a;
            break;
        // MOV r, M
        case 0x46:
            state.b = state.memory[state.hl];
            break;
        case 0x4E:
            state.c = state.memory[state.hl];
            break;
        case 0x56:
            state.d = state.memory[state.hl];
            break;
        case 0x5E:
            state.e = state.memory[state.hl];
            break;
        case 0x66:
            state.h = state.memory[state.hl];
            break;
        case 0x6E:
            state.l = state.memory[state.hl];
            break;
        case 0x7E:
            state.a = state.memory[state.hl];
            break;
        // MOV M, r
        case 0x70:
            state.memory[state.hl] = state.b;
            break;
        case 0x71:
            state.memory[state.hl] = state.c;
            break;
        case 0x72:
            state.memory[state.hl] = state.d;
            break;
        case 0x73:
            state.memory[state.hl] = state.e;
            break;
        case 0x74:
            state.memory[state.hl] = state.h;
            break;
        case 0x75:
            state.memory[state.hl] = state.l;
            break;
        case 0x77:
            state.memory[state.hl] = state.a;
            break;
        // MVI r, d8
        case 0x06:
            state.b = fetch_byte(state);
            break;
        case 0x0E:
            state.c = fetch_byte(state);
            break;
        case 0x16:
            state.d = fetch_byte(state);
            break;
        case 0x1E:
            state.e = fetch_byte(state);
            break;
        case 0x26:
            state.h = fetch_byte(state);
            break;
        case 0x2E:
            state.l = fetch_byte(state);
            break;
        case 0x3E:
            state.a = fetch_byte(state);
            break;
        // MVI M, d8
        case 0x36:
            state.memory[state.hl] = fetch_byte(state);
            break;
        // LXI rp, d16
        case 0x01: {
            uint16_t word = fetch_word(state);
            state.b = get_high_byte(word);
            state.c = get_low_byte(word);
            break;
        }
        case 0x11: {
            uint16_t word = fetch_word(state);
            state.d = get_high_byte(word);
            state.e = get_low_byte(word);
            break;
        }
        case 0x21: {
            uint16_t word = fetch_word(state);
            state.h = get_high_byte(word);
            state.l = get_low_byte(word);
            break;
        }
        case 0x31: {
            uint16_t word = fetch_word(state);
            state.sp = word;
            break;
        }
        // LDA a16
        case 0x3A: {
            uint16_t addr = fetch_word(state);
            state.a = state.memory[addr];
            break;
        }
        // STA a16
        case 0x32: {
            uint16_t addr = fetch_word(state);
            state.memory[addr] = state.a;
            break;
        }
        // LHLD a16
        case 0x2A: {
            uint16_t addr = fetch_word(state);
            state.l = state.memory[addr];
            state.h = state.memory[addr+1];
            break;
        }
        // SHLD a16
        case 0x22: {
            uint16_t addr = fetch_word(state);
            state.memory[addr] = state.l;
            state.memory[addr+1] = state.h;
            break;
        }
        // LDAX rp
        case 0x0A:
            state.a = state.memory[state.bc];
            break;
        case 0x1A:
            state.a = state.memory[state.de];
            break;
        // STAX rp
        case 0x02:
            state.memory[state.bc] = state.a;
            break;
        case 0x12:
            state.memory[state.de] = state.a;
            break;
        // XCHG
        case 0xEB:
            std::swap(state.hl, state.de);
            break;
        // ADD r
        case 0x80:
            inst_add(state, state.b);
            break;
        case 0x81:
            inst_add(state, state.c);
            break;
        case 0x82:
            inst_add(state, state.d);
            break;
        case 0x83:
            inst_add(state, state.e);
            break;
        case 0x84:
            inst_add(state, state.h);
            break;
        case 0x85:
            inst_add(state, state.l);
            break;
        case 0x87:
            inst_add(state, state.a);
            break;
        // ADD M
        case 0x86:
            inst_add(state, state.memory[state.hl]);
            break;
        // ADI d8
        case 0xC6:
            inst_add(state, fetch_byte(state));
            break;
        // ADC r
        case 0x88:
            inst_add(state, state.b, true);
            break;
        case 0x89:
            inst_add(state, state.c, true);
            break;
        case 0x8A:
            inst_add(state, state.d, true);
            break;
        case 0x8B:
            inst_add(state, state.e, true);
            break;
        case 0x8C:
            inst_add(state, state.h, true);
            break;
        case 0x8D:
            inst_add(state, state.l, true);
            break;
        case 0x8F:
            inst_add(state, state.a, true);
            break;
        // ADC M
        case 0x8E:
            inst_add(state, state.memory[state.hl], true);
            break;
        // ACI d8
        case 0xCE:
            inst_add(state, fetch_byte(state), true);
            break;
        // SUB r
        case 0x90:
            inst_sub(state, state.b);
            break;
        case 0x91:
            inst_sub(state, state.c);
            break;
        case 0x92:
            inst_sub(state, state.d);
            break;
        case 0x93:
            inst_sub(state, state.e);
            break;
        case 0x94:
            inst_sub(state, state.h);
            break;
        case 0x95:
            inst_sub(state, state.l);
            break;
        case 0x97:
            inst_sub(state, state.a);
            break;
        // SUB M
        case 0x96:
            inst_sub(state, state.memory[state.hl]);
            break;
        // SUI d8
        case 0xD6:
            inst_sub(state, fetch_byte(state));
            break;
        // SBB r
        case 0x98:
            inst_sub(state, state.b, true);
            break;
        case 0x99:
            inst_sub(state, state.c, true);
            break;
        case 0x9A:
            inst_sub(state, state.d, true);
            break;
        case 0x9B:
            inst_sub(state, state.e, true);
            break;
        case 0x9C:
            inst_sub(state, state.h, true);
            break;
        case 0x9D:
            inst_sub(state, state.l, true);
            break;
        case 0x9F:
            inst_sub(state, state.a, true);
            break;
        // SBB M
        case 0x9E:
            inst_sub(state, state.memory[state.hl], true);
            break;
        // SBI d8
        case 0xDE:
            inst_sub(state, fetch_byte(state), true);
            break;
        // INR r
        case 0x04:
            inst_inr(state, state.b);
            break;
        case 0x0C:
            inst_inr(state, state.c);
            break;
        case 0x14:
            inst_inr(state, state.d);
            break;
        case 0x1C:
            inst_inr(state, state.e);
            break;
        case 0x24:
            inst_inr(state, state.h);
            break;
        case 0x2C:
            inst_inr(state, state.l);
            break;
        case 0x3C:
            inst_inr(state, state.a);
            break;
        // INR M
        case 0x34:
            inst_inr(state, state.memory[state.hl]);
            break;
        // DCR r
        case 0x05:
            inst_dcr(state, state.b);
            break;
        case 0x0D:
            inst_dcr(state, state.c);
            break;
        case 0x15:
            inst_dcr(state, state.d);
            break;
        case 0x1D:
            inst_dcr(state, state.e);
            break;
        case 0x25:
            inst_dcr(state, state.h);
            break;
        case 0x2D:
            inst_dcr(state, state.l);
            break;
        case 0x3D:
            inst_dcr(state, state.a);
            break;
        // DCR M
        case 0x35:
            inst_dcr(state, state.memory[state.hl]);
            break;
        // INX rp
        case 0x03:
            state.bc++;
            break;
        case 0x13:
            state.de++;
            break;
        case 0x23:
            state.hl++;
            break;
        case 0x33:
            state.sp++;
            break;
        // DCX rp
        case 0x0B:
            state.bc--;
            break;
        case 0x1B:
            state.de--;
            break;
        case 0x2B:
            state.hl--;
            break;
        case 0x3B:
            state.sp--;
            break;
        // DAD rp
        case 0x09:
            inst_dad(state, state.bc);
            break;
        case 0x19:
            inst_dad(state, state.de);
            break;
        case 0x29:
            inst_dad(state, state.hl);
            break;
        case 0x39:
            inst_dad(state, state.sp);
            break;
        // DAA
        case 0x27:
            inst_daa(state);
            break;
        // ANA r
        case 0xA0:
            inst_and(state, state.b);
            break;
        case 0xA1:
            inst_and(state, state.c);
            break;
        case 0xA2:
            inst_and(state, state.d);
            break;
        case 0xA3:
            inst_and(state, state.e);
            break;
        case 0xA4:
            inst_and(state, state.h);
            break;
        case 0xA5:
            inst_and(state, state.l);
            break;
        case 0xA7:
            inst_and(state, state.a);
            break;
        // ANA M
        case 0xA6:
            inst_and(state, state.memory[state.hl]);
            break;
        // ANI d8
        case 0xE6:
            inst_and(state, fetch_byte(state), true);
            break;
        // XRA r
        case 0xA8:
            inst_xor(state, state.b);
            break;
        case 0xA9:
            inst_xor(state, state.c);
            break;
        case 0xAA:
            inst_xor(state, state.d);
            break;
        case 0xAB:
            inst_xor(state, state.e);
            break;
        case 0xAC:
            inst_xor(state, state.h);
            break;
        case 0xAD:
            inst_xor(state, state.l);
            break;
        case 0xAF:
            inst_xor(state, state.a);
            break;
        // XRA M
        case 0xAE:
            inst_xor(state, state.memory[state.hl]);
            break;
        // XRI d8
        case 0xEE:
            inst_xor(state, fetch_byte(state));
            break;
        // ORA r
        case 0xB0:
            inst_or(state, state.b);
            break;
        case 0xB1:
            inst_or(state, state.c);
            break;
        case 0xB2:
            inst_or(state, state.d);
            break;
        case 0xB3:
            inst_or(state, state.e);
            break;
        case 0xB4:
            inst_or(state, state.h);
            break;
        case 0xB5:
            inst_or(state, state.l);
            break;
        case 0xB7:
            inst_or(state, state.a);
            break;
        // ORA M
        case 0xB6:
            inst_or(state, state.memory[state.hl]);
            break;
        // ORI d8
        case 0xF6:
            inst_or(state, fetch_byte(state));
            break;
        // CMP r
        case 0xB8:
            inst_cmp(state, state.b);
            break;
        case 0xB9:
            inst_cmp(state, state.c);
            break;
        case 0xBA:
            inst_cmp(state, state.d);
            break;
        case 0xBB:
            inst_cmp(state, state.e);
            break;
        case 0xBC:
            inst_cmp(state, state.h);
            break;
        case 0xBD:
            inst_cmp(state, state.l);
            break;
        case 0xBF:
            inst_cmp(state, state.a);
            break;
        // CMP M
        case 0xBE:
            inst_cmp(state, state.memory[state.hl]);
            break;
        // CPI d8
        case 0xFE:
            inst_cmp(state, fetch_byte(state));
            break;
        // RLC
        case 0x07:
            inst_rlc(state);
            break;
        // RRC
        case 0x0F:
            inst_rrc(state);
            break;
        // RAL
        case 0x17:
            inst_ral(state);
            break;
        // RAR
        case 0x1F:
            inst_rar(state);
            break;
        // CMA
        case 0x2F:
            state.a = ~state.a;
            break;
        // CMC
        case 0x3F:
            set_flag(state, Flag::CY, !get_flag(state, Flag::CY));
            break;
        // STC
        case 0x37:
            set_flag(state, Flag::CY, true);
            break;
        // JMP a16
        case 0xC3:
            state.pc = fetch_word(state);
            break;
        // Jcondition a16
        case 0xC2: // JNZ
        {
            uint16_t addr = fetch_word(state);
            if (!get_flag(state, Flag::Z)) state.pc = addr;
            break;
        }
        case 0xCA: // JZ
        {
            uint16_t addr = fetch_word(state);
            if (get_flag(state, Flag::Z)) state.pc = addr;
            break;
        }
        case 0xD2: // JNC
        {
            uint16_t addr = fetch_word(state);
            if (!get_flag(state, Flag::CY)) state.pc = addr;
            break;
        }
        case 0xDA: // JC
        {
            uint16_t addr = fetch_word(state);
            if (get_flag(state, Flag::CY)) state.pc = addr;
            break;
        }
        case 0xE2: // JPO
        {
            uint16_t addr = fetch_word(state);
            if (!get_flag(state, Flag::P)) state.pc = addr;
            break;
        }
        case 0xEA: // JPE
        {
            uint16_t addr = fetch_word(state);
            if (get_flag(state, Flag::P)) state.pc = addr;
            break;
        }
        case 0xF2: // JP
        {
            uint16_t addr = fetch_word(state);
            if (!get_flag(state, Flag::S)) state.pc = addr;
            break;
        }
        case 0xFA: // JM
        {
            uint16_t addr = fetch_word(state);
            if (get_flag(state, Flag::S)) state.pc = addr;
            break;
        }
        // CALL a16
        case 0xCD:
        case 0xDD:
        case 0xED:
        case 0xFD:
            inst_call(state, true);
            break;
        // Ccondition a16
        case 0xC4: // CNZ
            inst_call(state, !get_flag(state, Flag::Z));
            break;
        case 0xCC: // CZ
            inst_call(state, get_flag(state, Flag::Z));
            break;
        case 0xD4: // CNC
            inst_call(state, !get_flag(state, Flag::CY));
            break;
        case 0xDC: // CC
            inst_call(state, get_flag(state, Flag::CY));
            break;
        case 0xE4: // CPO
            inst_call(state, !get_flag(state, Flag::P));
            break;
        case 0xEC: // CPE
            inst_call(state, get_flag(state, Flag::P));
            break;
        case 0xF4: // CP
            inst_call(state, !get_flag(state, Flag::S));
            break;
        case 0xFC: // CM
            inst_call(state, get_flag(state, Flag::S));
            break;
        // RET
        case 0xC9:
        case 0xD9:
            inst_ret(state, true);
            break;
        // Rcondition
        case 0xC0: // RNZ
            inst_ret(state, !get_flag(state, Flag::Z));
            break;
        case 0xC8: // RZ
            inst_ret(state, get_flag(state, Flag::Z));
            break;
        case 0xD0: // RNC
            inst_ret(state, !get_flag(state, Flag::CY));
            break;
        case 0xD8: // RC
            inst_ret(state, get_flag(state, Flag::CY));
            break;
        case 0xE0: // RPO
            inst_ret(state, !get_flag(state, Flag::P));
            break;
        case 0xE8: // RPE
            inst_ret(state, get_flag(state, Flag::P));
            break;
        case 0xF0: // RP
            inst_ret(state, !get_flag(state, Flag::S));
            break;
        case 0xF8: // RM
            inst_ret(state, get_flag(state, Flag::S));
            break;
        // RST n
        case 0xC7:
            inst_rst(state, 0);
            break;
        case 0xCF:
            inst_rst(state, 1);
            break;
        case 0xD7:
            inst_rst(state, 2);
            break;
        case 0xDF:
            inst_rst(state, 3);
            break;
        case 0xE7:
            inst_rst(state, 4);
            break;
        case 0xEF:
            inst_rst(state, 5);
            break;
        case 0xF7:
            inst_rst(state, 6);
            break;
        case 0xFF:
            inst_rst(state, 7);
            break;
        // PCHL
        case 0xE9:
            state.pc = state.hl;
            break;
        // PUSH rp
        case 0xC5:
            push_word(state, state.bc);
            break;
        case 0xD5:
            push_word(state, state.de);
            break;
        case 0xE5:
            push_word(state, state.hl);
            break;
        // PUSH PSW
        case 0xF5:
            push_word(state, (state.a << 8) | state.flags);
            break;
        // POP rp
        case 0xC1:
            state.bc = pop_word(state);
            break;
        case 0xD1:
            state.de = pop_word(state);
            break;
        case 0xE1:
            state.hl = pop_word(state);
            break;
        // POP PSW
        case 0xF1: {
            uint16_t psw = pop_word(state);
            state.flags = get_low_byte(psw);
            state.flags |= 0x02;
            state.flags &= ~0x28;
            state.a = get_high_byte(psw);
            break;
        }
        // XTHL
        case 0xE3:
            std::swap(state.l, state.memory[state.sp]);
            std::swap(state.h, state.memory[state.sp+1]);
            break;
        // SPHL
        case 0xF9:
            state.sp = state.hl;
            break;
        // IN port
        case 0xDB:
            std::cout << "IN (0xDB) not implemented\n";
            break;
        // OUT port
        case 0xD3:
            std::cout << "OUT (0xD3) not implemented\n";
            break;
        // EI
        case 0xFB:
            //std::cout << "EI (0xFB) not implemented\n";
            break;
        // DI
        case 0xF3:
            //std::cout << "DI (0xF3) not implemented\n";
            break;
        // HLT
        case 0x76:
            break;
    }
}

