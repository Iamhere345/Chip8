#include "chip8.h"
#include <cstring>

// Clear screen
void Chip8::CLS(uint16_t opcode) {
    //printf("0x%X implemented\n", opcode);

    memset(gfx, 0, 64 * 32);

    pc += 2;

    printf("clear screen\n", opcode);
}	

void Chip8::RET(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}	
void Chip8::SYS(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}	

// 0x1NNN: Non-conditional jump
void Chip8::JMP(uint16_t opcode) {
    //printf("0x%X implemented\n", opcode);
    printf("jump to 0x%X (pc: 0x%X)\n", opcode & 0x0FFF, pc);

    pc = opcode & 0x0FFF;

}

void Chip8::CALL(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}	

void Chip8::SE(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}	

void Chip8::SNE(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}

void Chip8::SEY(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}

// 0x6XKK sets Vx to u8 kk 
void Chip8::LD(uint16_t opcode) {
    //printf("0x%X implemented\n", opcode);

    uint16_t index = (opcode & 0x0F00) >> 8;
    V[index] = (opcode & 0x00FF) << 8;

    uint8_t vx = (opcode & 0x00FF);

    //printf("0x%X", vx);

    pc += 2;

    printf("set V[%d] to 0x%X (0x%X)\n", index, vx, opcode);
}	

// 0x7XNN adds NN to Vx
void Chip8::ADD(uint16_t opcode) {
    //printf("0x%X implemented\n", opcode);

    V[opcode & 0x0F00] += (opcode & 0x00FF) << 8;

    pc += 2;

    printf("add %d to V[%d]\n", (opcode & 0x00FF) << 8, opcode & 0x0F00);

}	

void Chip8::LDY(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}		

void Chip8::OR(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}

void Chip8::AND(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}

void Chip8::XOR(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}	

void Chip8::ADDY(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}

void Chip8::SUB(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}

void Chip8::SHR(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}	

void Chip8::SUBN(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
} 

void Chip8::SHL(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}

void Chip8::SNEY(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}

// ANNN sets index register I to NNN
void Chip8::ILDADDR(uint16_t opcode) {
    //printf("0x%X implemented\n", opcode);

    I = opcode & 0x0FFF;

    pc += 2;

    printf("set I to 0x%X\n", I);

}

void Chip8::JMPADDR(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}

void Chip8::RND(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}

// DXYN: 
void Chip8::DRW(uint16_t opcode) {
    printf("0x%X DRAW\n", opcode);

    pc += 2;
}

void Chip8::SKP(uint16_t opcode) {
    printf("0x%X unimplemented AA\n", opcode);
}

void Chip8::SKNP(uint16_t opcode) {
    printf("0x%X unimplemented AA\n", opcode);
} 

void Chip8::XLDDT(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}

void Chip8::XLDK(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}

void Chip8::DTLDX(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}

void Chip8::STLDX(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}

void Chip8::IADDX(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}

void Chip8::FLDX(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}

void Chip8::BLDX(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}

void Chip8::DILDX(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}

void Chip8::XLDDI(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);
}

