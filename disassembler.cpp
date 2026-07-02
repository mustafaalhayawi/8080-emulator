#include "disassembler.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

template <typename T>
std::string int_to_hex(const T val) {
    std::stringstream stream;
    stream << "0x" << std::setfill('0') << std::setw(sizeof(T)*2) << std::hex << static_cast<unsigned int>(val);
    return stream.str();
}

void disassemble_8080_op(const std::vector<uint8_t>& memory, uint16_t& pc) {
    std::string mnemonic = "", comment = "";
    uint16_t cur_pc = pc;

    switch (memory[pc]) {
        case 0x00:
            mnemonic = "NOP";
            pc++;
            break;
        case 0x01: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t val = low | (high << 8);
            mnemonic = "LXI B, " + int_to_hex(val);
            pc += 3;
            break;
        }
        case 0x02:
            mnemonic = "STAX B";
            pc++;
            break;
        case 0x03:
            mnemonic = "INX B";
            pc++;
            break;
        case 0x04:
            mnemonic = "INR B";
            pc++;
            break;
        case 0x05:
            mnemonic = "DCR B";
            pc++;
            break;
        case 0x06: {
            uint8_t val = memory[pc+1];
            mnemonic = "MVI B, " + int_to_hex(val);
            pc += 2;
            break;
        }
        case 0x07:
            mnemonic = "RLC";
            pc++;
            break;
        case 0x08:
            mnemonic = "NOP";
            comment = "0x08 alternative opcode to 0x00";
            pc++;
            break;
        case 0x09:
            mnemonic = "DAD B";
            pc++;
            break;
        case 0x0A:
            mnemonic = "LDAX B";
            pc++;
            break;
        case 0x0B:
            mnemonic = "DCX B";
            pc++;
            break;
        case 0x0C:
            mnemonic = "INR C";
            pc++;
            break;
        case 0x0D:
            mnemonic = "DCR C";
            pc++;
            break;
        case 0x0E: {
            uint8_t val = memory[pc+1];
            mnemonic = "MVI C, " + int_to_hex(val);
            pc += 2;
            break;
        }
        case 0x0F:
            mnemonic = "RRC";
            pc++;
            break;
        case 0x10:
            mnemonic = "NOP";
            comment = "0x01 alternative opcode to 0x00";
            pc++;
            break;
        case 0x11: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t val = low | (high << 8);
            mnemonic = "LXI D, " + int_to_hex(val);
            pc += 3;
            break;
        }
        case 0x12:
            mnemonic = "STAX D";
            pc++;
            break;
        case 0x13:
            mnemonic = "INX D";
            pc++;
            break;
        case 0x14:
            mnemonic = "INR D";
            pc++;
            break;
        case 0x15:
            mnemonic = "DCR D";
            pc++;
            break;
        case 0x16: {
            uint8_t val = memory[pc+1];
            mnemonic = "MVI D, " + int_to_hex(val);
            pc += 2;
            break;
        }
        case 0x17:
            mnemonic = "RAL";
            pc++;
            break;
        case 0x18:
            mnemonic = "NOP";
            comment = "0x18 alternative opcode to 0x00";
            pc++;
            break;
        case 0x19:
            mnemonic = "DAD D";
            pc++;
            break;
        case 0x1A:
            mnemonic = "LDAX D";
            pc++;
            break;
        case 0x1B:
            mnemonic = "DCX D";
            pc++;
            break;
        case 0x1C:
            mnemonic = "INR E";
            pc++;
            break;
        case 0x1D:
            mnemonic = "DCR E";
            pc++;
            break;
        case 0x1E: {
            uint8_t val = memory[pc+1];
            mnemonic = "MVI E, " + int_to_hex(val);
            pc += 2;
            break;
        }
        case 0x1F:
            mnemonic = "RAR";
            pc++;
            break;
        case 0x20:
            mnemonic = "NOP";
            comment = "0x20 alternative opcode to 0x00";
            pc++;
            break;
        case 0x21: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t val = low | (high << 8);
            mnemonic = "LXI H, " + int_to_hex(val);
            pc += 3;
            break;
        }
        case 0x22: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "SHLD " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0x23:
            mnemonic = "INX H";
            pc++;
            break;
        case 0x24:
            mnemonic = "INR H";
            pc++;
            break;
        case 0x25:
            mnemonic = "DCR H";
            pc++;
            break;
        case 0x26: {
            uint8_t val = memory[pc+1];
            mnemonic = "MVI H, " + int_to_hex(val);
            pc += 2;
            break;
        }
        case 0x27:
            mnemonic = "DAA";
            pc++;
            break;
        case 0x28:
            mnemonic = "NOP";
            comment = "0x28 alternative opcode to 0x00";
            pc++;
            break;
        case 0x29:
            mnemonic = "DAD H";
            pc++;
            break;
        case 0x2A: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "LHLD " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0x2B:
            mnemonic = "DCX H";
            pc++;
            break;
        case 0x2C:
            mnemonic = "INR L";
            pc++;
            break;
        case 0x2D:
            mnemonic = "DCR L";
            pc++;
            break;
        case 0x2E: {
            uint8_t val = memory[pc+1];
            mnemonic = "MVI L, " + int_to_hex(val);
            pc += 2;
            break;
        }
        case 0x2F:
            mnemonic = "CMA";
            pc++;
            break;
        case 0x30:
            mnemonic = "NOP";
            comment = "0x30 alternative opcode to 0x00";
            pc++;
            break;
        case 0x31: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "LXI SP, " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0x32: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "STA " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0x33:
            mnemonic = "INX SP";
            pc++;
            break;
        case 0x34:
            mnemonic = "INR M";
            pc++;
            break;
        case 0x35:
            mnemonic = "DCR M";
            pc++;
            break;
        case 0x36: {
            uint8_t val = memory[pc+1];
            mnemonic = "MVI M, " + int_to_hex(val);
            pc += 2;
            break;
        }
        case 0x37:
            mnemonic = "STC";
            pc++;
            break;
        case 0x38:
            mnemonic = "NOP";
            comment = "0x38 alternative opcode to 0x00";
            pc++;
            break;
        case 0x39:
            mnemonic = "DAD SP";
            pc++;
            break;
        case 0x3A: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "LDA " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0x3B:
            mnemonic = "DCX SP";
            pc++;
            break;
        case 0x3C:
            mnemonic = "INR A";
            pc++;
            break;
        case 0x3D:
            mnemonic = "DCR A";
            pc++;
            break;
        case 0x3E: {
            uint8_t val = memory[pc+1];
            mnemonic = "MVI A, " + int_to_hex(val);
            pc += 2;
            break;
        }
        case 0x3F:
            mnemonic = "CMC";
            pc++;
            break;
        case 0x40:
            mnemonic = "MOV B, B";
            pc++;
            break;
        case 0x41:
            mnemonic = "MOV B, C";
            pc++;
            break;
        case 0x42:
            mnemonic = "MOV B, D";
            pc++;
            break;
        case 0x43:
            mnemonic = "MOV B, E";
            pc++;
            break;
        case 0x44:
            mnemonic = "MOV B, H";
            pc++;
            break;
        case 0x45:
            mnemonic = "MOV B, L";
            pc++;
            break;
        case 0x46:
            mnemonic = "MOV B, M";
            pc++;
            break;
        case 0x47:
            mnemonic = "MOV B, A";
            pc++;
            break;
        case 0x48:
            mnemonic = "MOV C, B";
            pc++;
            break;
        case 0x49:
            mnemonic = "MOV C, C";
            pc++;
            break;
        case 0x4A:
            mnemonic = "MOV C, D";
            pc++;
            break;
        case 0x4B:
            mnemonic = "MOV C, E";
            pc++;
            break;
        case 0x4C:
            mnemonic = "MOV C, H";
            pc++;
            break;
        case 0x4D:
            mnemonic = "MOV C, L";
            pc++;
            break;
        case 0x4E:
            mnemonic = "MOV C, M";
            pc++;
            break;
        case 0x4F:
            mnemonic = "MOV C, A";
            pc++;
            break;
        case 0x50:
            mnemonic = "MOV D, B";
            pc++;
            break;
        case 0x51:
            mnemonic = "MOV D, C";
            pc++;
            break;
        case 0x52:
            mnemonic = "MOV D, D";
            pc++;
            break;
        case 0x53:
            mnemonic = "MOV D, E";
            pc++;
            break;
        case 0x54:
            mnemonic = "MOV D, H";
            pc++;
            break;
        case 0x55:
            mnemonic = "MOV D, L";
            pc++;
            break;
        case 0x56:
            mnemonic = "MOV D, M";
            pc++;
            break;
        case 0x57:
            mnemonic = "MOV D, A";
            pc++;
            break;
        case 0x58:
            mnemonic = "MOV E, B";
            pc++;
            break;
        case 0x59:
            mnemonic = "MOV E, C";
            pc++;
            break;
        case 0x5A:
            mnemonic = "MOV E, D";
            pc++;
            break;
        case 0x5B:
            mnemonic = "MOV E, E";
            pc++;
            break;
        case 0x5C:
            mnemonic = "MOV E, H";
            pc++;
            break;
        case 0x5D:
            mnemonic = "MOV E, L";
            pc++;
            break;
        case 0x5E:
            mnemonic = "MOV E, M";
            pc++;
            break;
        case 0x5F:
            mnemonic = "MOV E, A";
            pc++;
            break;
        case 0x60:
            mnemonic = "MOV H, B";
            pc++;
            break;
        case 0x61:
            mnemonic = "MOV H, C";
            pc++;
            break;
        case 0x62:
            mnemonic = "MOV H, D";
            pc++;
            break;
        case 0x63:
            mnemonic = "MOV H, E";
            pc++;
            break;
        case 0x64:
            mnemonic = "MOV H, H";
            pc++;
            break;
        case 0x65:
            mnemonic = "MOV H, L";
            pc++;
            break;
        case 0x66:
            mnemonic = "MOV H, M";
            pc++;
            break;
        case 0x67:
            mnemonic = "MOV H, A";
            pc++;
            break;
        case 0x68:
            mnemonic = "MOV L, B";
            pc++;
            break;
        case 0x69:
            mnemonic = "MOV L, C";
            pc++;
            break;
        case 0x6A:
            mnemonic = "MOV L, D";
            pc++;
            break;
        case 0x6B:
            mnemonic = "MOV L, E";
            pc++;
            break;
        case 0x6C:
            mnemonic = "MOV L, H";
            pc++;
            break;
        case 0x6D:
            mnemonic = "MOV L, L";
            pc++;
            break;
        case 0x6E:
            mnemonic = "MOV L, M";
            pc++;
            break;
        case 0x6F:
            mnemonic = "MOV L, A";
            pc++;
            break;
        case 0x70:
            mnemonic = "MOV M, B";
            pc++;
            break;
        case 0x71:
            mnemonic = "MOV M, C";
            pc++;
            break;
        case 0x72:
            mnemonic = "MOV M, D";
            pc++;
            break;
        case 0x73:
            mnemonic = "MOV M, E";
            pc++;
            break;
        case 0x74:
            mnemonic = "MOV M, H";
            pc++;
            break;
        case 0x75:
            mnemonic = "MOV M, L";
            pc++;
            break;
        case 0x76:
            mnemonic = "HLT";
            pc++;
            break;
        case 0x77:
            mnemonic = "MOV M, A";
            pc++;
            break;
        case 0x78:
            mnemonic = "MOV A, B";
            pc++;
            break;
        case 0x79:
            mnemonic = "MOV A, C";
            pc++;
            break;
        case 0x7A:
            mnemonic = "MOV A, D";
            pc++;
            break;
        case 0x7B:
            mnemonic = "MOV A, E";
            pc++; 
            break;
        case 0x7C:
            mnemonic = "MOV A, H";
            pc++;
            break;
        case 0x7D:
            mnemonic = "MOV A, L";
            pc++;
            break;
        case 0x7E:
            mnemonic = "MOV A, M";
            pc++;
            break;
        case 0x7F:
            mnemonic = "MOV A, A";
            pc++;
            break;
        case 0x80:
            mnemonic = "ADD B";
            pc++;
            break;
        case 0x81:
            mnemonic = "ADD C";
            pc++;
            break;
        case 0x82:
            mnemonic = "ADD D";
            pc++;
            break;
        case 0x83:
            mnemonic = "ADD E";
            pc++; 
            break;
        case 0x84:
            mnemonic = "ADD H";
            pc++;
            break;
        case 0x85:
            mnemonic = "ADD L";
            pc++;
            break;
        case 0x86:
            mnemonic = "ADD M";
            pc++;
            break;
        case 0x87:
            mnemonic = "ADD A";
            pc++;
            break;
        case 0x88:
            mnemonic = "ADC B";
            pc++;
            break;
        case 0x89:
            mnemonic = "ADC C";
            pc++;
            break;
        case 0x8A:
            mnemonic = "ADC D";
            pc++;
            break;
        case 0x8B:
            mnemonic = "ADC E";
            pc++;
            break;
        case 0x8C:
            mnemonic = "ADC H";
            pc++;
            break;
        case 0x8D:
            mnemonic = "ADC L";
            pc++;
            break;
        case 0x8E:
            mnemonic = "ADC M";
            pc++;
            break;
        case 0x8F:
            mnemonic = "ADC A";
            pc++;
            break;
        case 0x90:
            mnemonic = "SUB B";
            pc++;
            break;
        case 0x91:
            mnemonic = "SUB C";
            pc++;
            break;
        case 0x92:
            mnemonic = "SUB D";
            pc++;
            break;
        case 0x93:
            mnemonic = "SUB E";
            pc++;
            break;
        case 0x94:
            mnemonic = "SUB H";
            pc++;
            break;
        case 0x95:
            mnemonic = "SUB L";
            pc++;
            break;
        case 0x96:
            mnemonic = "SUB M";
            pc++;
            break;
        case 0x97:
            mnemonic = "SUB A";
            pc++;
            break;
        case 0x98:
            mnemonic = "SBB B";
            pc++;
            break;
        case 0x99:
            mnemonic = "SBB C"; 
            pc++;
            break;
        case 0x9A:
            mnemonic = "SBB D";
            pc++;
            break;
        case 0x9B:
            mnemonic = "SBB E";
            pc++;
            break;
        case 0x9C:
            mnemonic = "SBB H";
            pc++;
            break;
        case 0x9D:
            mnemonic = "SBB L";
            pc++;
            break;
        case 0x9E:
            mnemonic = "SBB M";
            pc++;
            break;
        case 0x9F:
            mnemonic = "SBB A";
            pc++;
            break;
        case 0xA0:
            mnemonic = "ANA B";
            pc++;
            break;
        case 0xA1:
            mnemonic = "ANA C";
            pc++;
            break;
        case 0xA2:
            mnemonic = "ANA D";
            pc++;
            break;
        case 0xA3:
            mnemonic = "ANA E";
            pc++;
            break;
        case 0xA4:
            mnemonic = "ANA H";
            pc++;
            break;
        case 0xA5:
            mnemonic = "ANA L";
            pc++;
            break;
        case 0xA6:
            mnemonic = "ANA M";
            pc++;
            break;
        case 0xA7:
            mnemonic = "ANA A";
            pc++;
            break;
        case 0xA8:
            mnemonic = "XRA B";
            pc++;
            break;
        case 0xA9:
            mnemonic = "XRA C";
            pc++;
            break;
        case 0xAA:
            mnemonic = "XRA D";
            pc++;
            break;
        case 0xAB:
            mnemonic = "XRA E";
            pc++;
            break;
        case 0xAC:
            mnemonic = "XRA H";
            pc++;
            break;
        case 0xAD:
            mnemonic = "XRA L";
            pc++;
            break;
        case 0xAE:
            mnemonic = "XRA M";
            pc++;
            break;
        case 0xAF:
            mnemonic = "XRA A";
            pc++;
            break;
        case 0xB0:
            mnemonic = "ORA B";
            pc++;
            break;
        case 0xB1:
            mnemonic = "ORA C";
            pc++;
            break;
        case 0xB2:
            mnemonic = "ORA D";
            pc++;
            break;
        case 0xB3:
            mnemonic = "ORA E";
            pc++;
            break;
        case 0xB4:
            mnemonic = "ORA H";
            pc++;
            break;
        case 0xB5:
            mnemonic = "ORA L";
            pc++;
            break;
        case 0xB6:
            mnemonic = "ORA M";
            pc++;
            break;
        case 0xB7:
            mnemonic = "ORA A";
            pc++;
            break;
        case 0xB8:
            mnemonic = "CMP B";
            pc++;
            break;
        case 0xB9:
            mnemonic = "CMP C";
            pc++;
            break;
        case 0xBA:
            mnemonic = "CMP D";
            pc++;
            break;
        case 0xBB:
            mnemonic = "CMP E";
            pc++;
            break;
        case 0xBC:
            mnemonic = "CMP H";
            pc++;
            break;
        case 0xBD:
            mnemonic = "CMP L";
            pc++;
            break;
        case 0xBE:
            mnemonic = "CMP M";
            pc++;
            break;
        case 0xBF:
            mnemonic = "CMP A";
            pc++;
            break;
        case 0xC0:
            mnemonic = "RNZ";
            pc++;
            break;
        case 0xC1:
            mnemonic = "POP B";
            pc++;
            break;
        case 0xC2: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "JNZ " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0xC3: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "JMP " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0xC4: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "CNZ " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0xC5:
            mnemonic = "PUSH B";
            pc++;
            break;
        case 0xC6: {
            uint8_t val = memory[pc+1];
            mnemonic = "ADI " + int_to_hex(val);
            pc += 2;
            break;
        }
        case 0xC7:
            mnemonic = "RST 0";
            pc++;
            break;
        case 0xC8:
            mnemonic = "RZ";
            pc++;
            break;
        case 0xC9:
            mnemonic = "RET";
            pc++;
            break;
        case 0xCA: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "JZ " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0xCB: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "JMP " + int_to_hex(addr);
            comment = "0xcb alternative opcode to 0xc3";
            pc += 3;
            break;
        }
        case 0xCC: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "CZ " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0xCD: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "CALL " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0xCE: {
            uint8_t val = memory[pc+1];
            mnemonic = "ACI " + int_to_hex(val);
            pc += 2;
            break;
        }
        case 0xCF:
            mnemonic = "RST 1";
            pc++;
            break;
        case 0xD0:
            mnemonic = "RNC";
            pc++;
            break;
        case 0xD1:
            mnemonic = "POP D";
            pc++;
            break;
        case 0xD2: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "JNC " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0xD3: {
            uint8_t val = memory[pc+1];
            mnemonic = "OUT " + int_to_hex(val);
            pc += 2;
            break;
        }
        case 0xD4: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "CNC " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0xD5:
            mnemonic = "PUSH D";
            pc++;
            break;
        case 0xD6: {
            uint8_t val = memory[pc+1];
            mnemonic = "SUI " + int_to_hex(val);
            pc += 2;
            break;
        }
        case 0xD7:
            mnemonic = "RST 2";
            pc++;
            break;
        case 0xD8:
            mnemonic = "RC";
            pc++;
            break;
        case 0xD9:
            mnemonic = "RET";
            comment = "0xD9 alternative opcode to 0xC9";
            pc++;
            break;
        case 0xDA: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "JC " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0xDB: {
            uint8_t val = memory[pc+1];
            mnemonic = "IN " + int_to_hex(val);
            pc += 2;
            break;
        }
        case 0xDC: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "CPE " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0xDD: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "CALL " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0xDE: {
            uint8_t val = memory[pc+1];
            mnemonic = "SBI " + int_to_hex(val);
            pc += 2;
            break;
        }
        case 0xDF:
            mnemonic = "RST 3";
            pc++;
            break;
        case 0xE0:
            mnemonic = "RPO";
            pc++;
            break;
        case 0xE1:
            mnemonic = "POP H";
            pc++;
            break;
        case 0xE2: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "JPO " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0xE3:
            mnemonic = "XTHL";
            pc++;
            break;
        case 0xE4: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "CPO " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0xE5:
            mnemonic = "PUSH H";
            pc++;
            break;
        case 0xE6: {
            uint8_t val = memory[pc+1];
            mnemonic = "ANI " + int_to_hex(val);
            pc += 2;
            break;
        }
        case 0xE7:
            mnemonic = "RST 4";
            pc++;
            break;
        case 0xE8:
            mnemonic = "RPE";
            pc++;
            break;
        case 0xE9:
            mnemonic = "PCHL";
            pc++;
            break;
        case 0xEA: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "JPE " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0xEB:
            mnemonic = "XCHG";
            pc++;
            break;
        case 0xEC: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "CPE " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0xED: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "CALL " + int_to_hex(addr);
            comment = "0xed alternative opcode to 0xcd";
            pc += 3;
            break;
        }
        case 0xEE: {
            uint8_t val = memory[pc+1];
            mnemonic = "XRI " + int_to_hex(val);
            pc += 2;
            break;
        }
        case 0xEF:
            mnemonic = "RST 5";
            pc++;
            break;
        case 0xF0:
            mnemonic = "RP";
            pc++;
            break;
        case 0xF1:
            mnemonic = "POP PSW";
            pc++;
            break;
        case 0xF2: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint8_t addr = low | (high << 8);
            mnemonic = "JP " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0xF3:
            mnemonic = "DI";
            pc++;
            break;
        case 0xF4: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "CP " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0xF5:
            mnemonic = "PUSH PSW";
            pc++;
            break;
        case 0xF6: {
            uint8_t val = memory[pc+1];
            mnemonic = "ORI " + int_to_hex(val);
            pc += 2;
            break;
        }
        case 0xF7:
            mnemonic = "RST 6";
            pc++;
            break;
        case 0xF8:
            mnemonic = "RM";
            pc++;
            break;
        case 0xF9:
            mnemonic = "SPHL";
            pc++;
            break;
        case 0xFA: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "JM " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0xFB:
            mnemonic = "EI";
            pc++;
            break;
        case 0xFC: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "CM " + int_to_hex(addr);
            pc += 3;
            break;
        }
        case 0xFD: {
            uint8_t low = memory[pc+1];
            uint8_t high = memory[pc+2];
            uint16_t addr = low | (high << 8);
            mnemonic = "CALL " + int_to_hex(addr);
            comment = "0xfd alternative opcode to 0xcd";
            pc += 3;
            break;
        }
        case 0xFE: {
            uint8_t val = memory[pc+1];
            mnemonic = "CPI " + int_to_hex(val);
            pc += 2;
            break;
        }
        case 0xFF:
            mnemonic = "RST 7";
            pc++;
            break;
    }

    std::cout << int_to_hex<uint16_t>(cur_pc + 0x0100) << ":\t" << mnemonic << ((comment == "") ? "" : ("\t; " + comment)) << "\n";
}

void disassemble_file(const std::string& filename) {
    std::vector<uint8_t> memory;
    std::ifstream infile;
    infile.open(filename, std::ios::binary | std::ios::in);
    infile.seekg(0, std::ios::end);
    int filesize = infile.tellg();
    memory.resize(filesize);
    infile.seekg(0);
    infile.read(reinterpret_cast<char*>(memory.data()), filesize);
    for (uint16_t pc=0; pc<memory.size(); ) {
        uint16_t cur_pc = pc;
        disassemble_8080_op(memory, pc);
    }
}
