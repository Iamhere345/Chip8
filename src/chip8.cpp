#include "chip8.h"
#include <vector>
#include <string.h>
#include <stdlib.h>

using namespace std;

enum Memonics {
    ANNN = 0xA000,
};

Chip8::Chip8() {
    
    pc      = 0x200;
    opcode  = 0;
    I       = 0;
    sp      = 0;

    // clear screen
    memset(gfx, 0, 64 * 32);

    for (int i = 10; i < 40; i++)
        gfx[i] = 1;

    // clear stack
    memset(stack, 0, 16);

    // clear registers
    memset(V, 0, 16);

    // clear memory
    memset(memory, 0, 4096);

    // load fontset
    for (int i = 0; i < 80; i++)
        memory[i] = FONTSET[i];

    // reset timers
    delay_timer = 0;
    sound_timer = 0;

}

bool Chip8::load_game(char* path) {

    FILE* rom = fopen(path, "rb");

    if (rom == NULL) {
        return false;
    }

    int c;
    for (int i = 0x200; (c = fgetc(rom)) != EOF && i < 4096; i++) {
        memory[i] = c;
    }

    return true;

}

void Chip8::cycle() {

    // fetch instruction
    opcode = memory[pc] << 8 | memory[pc + 1];

    // decode and execute instruction
    // TODO remove and replace with lookup table code

    bool opcode_executed = false;
    for (int i = 0; i < 35; i++) {

        LOOKUP_INFO info = LOOKUP_TABLE[i];

        if ((opcode & info.mask) == info.opcode) {

            //printf("0x%X & 0x%X == 0x%X\n", opcode, info.mask, info.opcode);

            // i hate this
            (*this.*(info.instruction))(opcode);
            opcode_executed = true;
            break;
        }
    }

    if (!opcode_executed) {
        printf("Undefined opcode 0x%X\n", opcode);
        pc += 2;
    }

    // tick timers
    // TODO timers need to be ticked at 60Hz
    if (delay_timer > 0) 
        delay_timer--;

    if (sound_timer > 0) {
        if (sound_timer == 1) 
            printf("BEEP!\n");
        sound_timer--;
    }

}