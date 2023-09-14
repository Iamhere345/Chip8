#include "chip8.h"
#include <cstring>

// 0x00E0: Clear screen
void Chip8::CLS(uint16_t opcode) {
    //printf("0x%X implemented\n", opcode);

    memset(gfx, 0, 64 * 32);

    pc += 2;

    printf("[0x%X] clear screen\n", opcode);
}	

// 0x0000: this instruction is depcrecated and should not be called
void Chip8::SYS(uint16_t opcode) {
    //printf("0x%X use of SYS is deprecated\n", opcode);

    dump_memory("build/memory.bin");

    //pc += 2;
}	

// 0x1NNN: Non-conditional jump
void Chip8::JMP(uint16_t opcode) {
    //printf("0x%X implemented\n", opcode);
    //printf("jump to 0x%X (pc: 0x%X)\n", opcode & 0x0FFF, pc);

    pc = opcode & 0x0FFF;

}

// 0x2NNN: add pc to the stack and jump to NNN
void Chip8::CALL(uint16_t opcode) {
    printf("[0x%X] CALL 0x%X from 0x%X\n", opcode, opcode & 0x0FFF, pc);

    stack[sp++] = pc;

    /*
    printf("STACK (current pos %d):\n", sp);

    for (int i = 0; i < 16; i++)
        printf("%d: 0x%X\n", i, stack[i]);
    */

    pc = opcode & 0x0FFF;

}

// 00EE: set pc to stack[sp] and pop the stack
void Chip8::RET(uint16_t opcode) {
    printf("RET 0x%X\n", stack[sp - 1]);

    /*
    printf("STACK (current pos %d):\n", sp);

    for (int i = 0; i < 16; i++)
        printf("%d: 0x%X\n", i, stack[i]);
    */

    pc = stack[--sp];

    printf("PC: 0x%X\n", pc);

    pc += 2;

}

// 0x3XNN: skip one instruction if Vx == NN
void Chip8::SE(uint16_t opcode) {
    printf("[0x%X] SE %d == %d\n", opcode, V[(opcode & 0x0F00) >> 8], opcode & 0x00FF);

    if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
        //printf("SE JMP\n");
        pc += 4;
    } else {
        //printf("SE NO JMP\n");
        pc += 2;
    }

}	

// 0x4XNN: skip one instruction if Vx != NN
void Chip8::SNE(uint16_t opcode) {
    printf("[0x%X] SNE %d != %d\n", opcode, V[(opcode & 0x0F00) >> 8], opcode & 0x00FF);

    if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
        //printf("SE JMP\n");
        pc += 4;
    } else {
        //printf("SE NO JMP\n");
        pc += 2;
    }
}

// 0x5XY0: skip one instruction if Vx == Vy
void Chip8::SEY(uint16_t opcode) {
    printf("[0x%X] SEY %d == %d\n", opcode, V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4]);

    if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
        //printf("SE JMP\n");
        pc += 4;
    } else {
        //printf("SE NO JMP\n");
        pc += 2;
    }
}

// 0x9XY0: skip one instruction if Vx != Vy
void Chip8::SNEY(uint16_t opcode) {
    printf("[0x%X] SNEY %d != %d\n", opcode, V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4]);

    if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
        //printf("SE JMP\n");
        pc += 4;
    } else {
        //printf("SE NO JMP\n");
        pc += 2;
    }
}

// 0x6XKK: set Vx to u8 kk 
void Chip8::LD(uint16_t opcode) {
    //printf("0x%X implemented\n", opcode);

    uint16_t index = (opcode & 0x0F00) >> 8;
    V[index] = (opcode & 0x00FF);

    uint8_t vx = (opcode & 0x00FF);

    //printf("0x%X", vx);

    pc += 2;

    printf("set V[%d] to 0x%X (0x%X)\n", index, vx, opcode);
}	

// 0x7XNN: add NN to Vx
void Chip8::ADD(uint16_t opcode) {
    //printf("0x%X ADD\n", opcode);

    V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);

    pc += 2;

    printf("add 0x%X to V[%d]\n", (opcode & 0x00FF), (opcode & 0x0F00) >> 8);

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

// 0xANNN: set index register I to NNN
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

// DXYN: draws a sprite to the screen at positon (Vx, Vy) that is N pixels tall
void Chip8::DRW(uint16_t opcode) {
    printf("0x%X DRAW\n", opcode);

    uint8_t x = V[(opcode & 0x0F00) >> 8] % 64;
    uint8_t y = V[(opcode & 0x00F0) >> 4] % 32;

    V[0xF] = 0;

    uint8_t rows = opcode & 0x000F;
    
    //printf("rows: %d", rows);

    for (int yline = 0; yline < rows; yline++) {

        uint8_t pixel = memory[I + yline];

        for (int xline = 0; xline < 8; xline++) {

            // 0x80 = 0b10000000
            uint8_t xbit = pixel & (0x80 >> xline);
            uint8_t screen_pixel = x + xline + ((y + yline) * 64);

            if (xbit != 0) {

                // if the pixel will be set to 0 set VF
                if (gfx[screen_pixel] == 1)
                    V[0xF] = 1;
                
                gfx[x + xline + ((y + yline) * 64)] ^= 1;
            } 

        }

    }

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

