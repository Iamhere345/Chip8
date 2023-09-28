#include "chip8.h"
#include <cstring>
#include <stdlib.h>

// 0x00E0: Clear screen
void Chip8::CLS(uint16_t opcode) {
    memset(gfx, 0, 64 * 32);

    pc += 2;

    printf("[0x%X] clear screen\n", opcode);
}	

// 0x0000: this instruction is depcrecated and should not be called
void Chip8::SYS(uint16_t opcode) {
    printf("0x%X use of SYS is deprecated\n", opcode);

    dump_memory("build/memory.bin");

    halt = true;
}	

// 0x1NNN: Non-conditional jump
void Chip8::JMP(uint16_t opcode) {
    printf("jump to 0x%X (pc: 0x%X)\n", opcode & 0x0FFF, pc);

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
    printf("SNE V[%d] %d != %d\n", (opcode & 0x0F00) >> 8, V[(opcode & 0x0F00) >> 8], opcode & 0x00FF);

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
    printf("SEY %d == %d\n", V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4]);

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
    printf("SNEY %d != %d\n", V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4]);

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

// 0x8XY0: set Vx to Vy
void Chip8::LDY(uint16_t opcode) {
    //printf("0x%X unimplemented\n", opcode);

    printf("Set V[%d] to V[%d]\n", (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);

    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];

    pc += 2;

}

// ? does not set VF on overflow
// 0x7XNN: add NN to Vx
void Chip8::ADD(uint16_t opcode) {
    //printf("0x%X ADD\n", opcode);

    V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);

    pc += 2;

    printf("add 0x%X to V[%d]\n", (opcode & 0x00FF), (opcode & 0x0F00) >> 8);

}	

// ? VF undefined
// 0x8XY1: set Vx to Vx | Vy
void Chip8::OR(uint16_t opcode) {

    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];

    pc += 2;

}

// ? VF undefined
// 0x8XY2: set Vx to Vx & Vy
void Chip8::AND(uint16_t opcode) {

    printf("V[%d] = 0x%X & 0x%X == 0x%X\n", (opcode & 0x0F00) >> 8, V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4], V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4]);

    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];

    pc += 2;

}

// ? VF undefined
// 0x8XY3: set Vx to Vx ^ Vy
void Chip8::XOR(uint16_t opcode) {

    printf("V[%d] = 0x%X ^ 0x%X == 0x%X\n", (opcode & 0x0F00) >> 8, V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4], V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4]);

    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];

    pc += 2;
}	

// ? sets VF on overfow
// 0x8XY4: set Vx to Vx + Vy
void Chip8::ADDY(uint16_t opcode) {
    printf("V[%d] = 0x%X + 0x%X == 0x%X\n", (opcode & 0x0F00) >> 8, V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4], V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4]);

    uint16_t vx = (opcode & 0x0F00) >> 8;
    uint16_t vy = (opcode & 0x00F0) >> 4;

    if ((uint16_t) V[vx] + (uint16_t) V[vy] > 255) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }

    V[vx] = V[vx] + V[vy];

    pc += 2;
}

// ? sets VF to 1 if Vx > Vy, otherwise VF is set to 0 (underflow)
// 0x8XY5: set Vx to Vx - Vy
void Chip8::SUB(uint16_t opcode) {
    printf("V[%d] = 0x%X - 0x%X == 0x%X\n", (opcode & 0x0F00) >> 8, V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4], V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4]);

    uint16_t vx = (opcode & 0x0F00) >> 8;
    uint16_t vy = (opcode & 0x00F0) >> 4;

    if (V[vx] > V[vy]) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }

    V[vx] = V[vx] - V[vy];

    pc += 2;
}

// 0x8XY7: set Vx to Vy - Vx
void Chip8::SUBN(uint16_t opcode) {
    printf("V[%d] = 0x%X - 0x%X == 0x%X\n", (opcode & 0x0F00) >> 8, V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4], V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4]);

    uint16_t vx = (opcode & 0x0F00) >> 8;
    uint16_t vy = (opcode & 0x00F0) >> 4;

    if (V[vy] > V[vx]) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }

    V[vx] = V[vy] - V[vx];

    pc += 2;
} 

// ? SHL and SHR will have to be modified for an SCHIP or CHIP-48 Implementation
// 0x8XYE: set Vx to Vy (optional) and shift Vx to the left
void Chip8::SHL(uint16_t opcode) {
    printf("SHL\n");

    if ((V[(opcode & 0x00F0) >> 4] & 0x80) != 0) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }

    V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x00F0) >> 4]) << 1;

    pc += 2;

}

// 0x8XYE: set Vx to Vy (optional) and shift Vx to the left
void Chip8::SHR(uint16_t opcode) {
    printf("SHR\n");

    if ((V[(opcode & 0x00F0) >> 4] & 0x1) != 0) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }

    V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x00F0) >> 4]) >> 1;

    pc += 2;

}

// 0xANNN: set index register I to NNN
void Chip8::LDI(uint16_t opcode) {

    I = opcode & 0x0FFF;

    pc += 2;

    printf("set I to 0x%X\n", I);

}

// ? this will need to be changed for an SCHIP or CHIP-48 implementation
// 0xBNNN: set pc to NNN + V0
void Chip8::RJMP(uint16_t opcode) {
    printf("relative jump to 0x%X (0x%X + 0x%X)", (opcode & 0x0FFF) + V[0], opcode & 0x0FFF, V[0]);

	pc = (opcode & 0x0FFF) + V[0];
}

// 0xCXNN: generates a random number, ands it with NN and stores the result in Vx
void Chip8::RND(uint16_t opcode) {
    
	int rnd = rand();

	printf("rnd num is %d (0x%X) masked: 0x%X\n", rnd, rnd, rnd & (opcode & 0x0FF));

	V[(opcode & 0x0F00) >> 8] = rnd & (opcode & 0x0FF);

	pc += 2;

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

// 0xEX9E: skips an instruction if the key held in Vx is pressed
void Chip8::KEY(uint16_t opcode) {
    uint8_t key_pressed = key[V[(opcode & 0x0F00) >> 8]];

    if (key_pressed) {
        printf("key 0x%X pressed. SKIP.\n", V[(opcode & 0x0F00) >> 8]);
        pc += 4;
    } else {
        printf("key 0x%X not pressed\n", V[(opcode & 0x0F00) >> 8]);
        pc += 2;
    }

}

// 0xEXA1: skips an instruction if the key held in Vx is not pressed
void Chip8::NKEY(uint16_t opcode) {
    uint8_t key_pressed = key[V[(opcode & 0x0F00) >> 8]];

    if (!key_pressed) {
        printf("key 0x%X not pressed. SKIP.\n", V[(opcode & 0x0F00) >> 8]);
        pc += 4;
    } else {
        printf("key 0x%X pressed\n", V[(opcode & 0x0F00) >> 8]);
        pc += 2;
    }

} 

void Chip8::XLDDT(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);

    halt = true;
}

void Chip8::XLDK(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);

    halt = true;
}

void Chip8::DTLDX(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);

    halt = true;
}

void Chip8::STLDX(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);

    halt = true;
}

// ? although it wasn't in the original implementation, some games rely on VF being set if I overflows
// 0xFX1E: add Vx to I
void Chip8::ADDI(uint16_t opcode) {
    printf("I (0x%X) += 0x%X", I, (opcode & 0x0F00) >> 8);

    I += (opcode & 0x0F00) >> 8;

    if (I >= 0x1000) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }

    pc += 2;

}

void Chip8::FLDX(uint16_t opcode) {
    printf("0x%X unimplemented\n", opcode);

    halt = true;
}

// 0xFX33: takes the number in Vx, and splits each of its three digits (8 bits = 0-255) and puts them in I, I+1, I+2
void Chip8::DEC(uint16_t opcode) {

	uint8_t vx = V[(opcode & 0x0F00) >> 8];	

	memory[I] = vx / 100;
	memory[I + 1] = (vx / 10) % 10;
	memory[I + 2] = (vx % 100) % 10;

	pc += 2;

}

// 0xFX55: store registers V0-Vx into memory, starting at I
void Chip8::STX(uint16_t opcode) {

    uint16_t vx = (opcode & 0x0F00) >> 8;

    printf("store registers V[0]-V[%d] in 0x%X\n", vx, I);

    for (int i = 0; i <= vx; i++) {

        printf("memory[0x%X + %d] = V[%d]\n", I, i, i);
        memory[I + i] = V[i];

    }

    pc += 2;

}

// 0xFX65: load memory starting at I into registers V0-Vx
void Chip8::LDX(uint16_t opcode) {
    
    uint16_t vx = (opcode & 0x0F00) >> 8;

    printf("load 0x%X-0x%X into registers V[0]-V[%d]\n", I, I + vx, vx);

    for (int i = 0; i <= vx; i++) {

		printf("V[%d] = memory[0x%X + %d]", i, I, i);
		V[i] = memory[I + i];

    }

    pc += 2;

}

