#include "chip8.h"
#include <vector>
#include <string.h>
#include <stdlib.h>

using namespace std;

Chip8::Chip8() {
    
    pc      = 0x200;
    opcode  = 0;
    I       = 0;
    sp      = 0;

    // initialise memory
    memset(gfx, 0, 64 * 32);
    memset(stack, 0, 16 * sizeof(uint16_t));
    memset(V, 0, 16);
    memset(memory, 0, 4096);

    // load fontset
    for (int i = 0; i < 80; i++)
        memory[i] = FONTSET[i];

    // reset timers
    delay_timer = 0;
    sound_timer = 0;

    halt = false;

}

bool Chip8::load_game(char* path) {

    FILE* rom = fopen(path, "rb");

    if (rom == NULL) {
        return false;
    }

    int c, i;
    for (i = 0x200; (c = fgetc(rom)) != EOF && i < 4096; i++) {
        memory[i] = c;
    }

    printf("**** %d bytes loaded ****\n", i - 0x200);

    return true;

}

bool Chip8::dump_memory(char* path) {

    FILE* rom = fopen(path, "w");

    if (rom == NULL) {
        return false;
    }

    for (int i = 0; i < 4096; i++)
        fputc(memory[i], rom);

    fclose(rom);

    return true;

}

void Chip8::cycle() {

    static bool halt_dump = false;

    if (halt) {
        if (!halt_dump) {
            this->dump_memory("build/memory.bin");
            halt_dump = true;
        }

        return;
    }

    // fetch instruction
    opcode = memory[pc] << 8 | memory[pc + 1];

    // decode and execute instruction
    bool opcode_executed = false;
    for (int i = 0; i < 35; i++) {

        LOOKUP_INFO info = LOOKUP_TABLE[i];

        if ((opcode & info.mask) == info.opcode) {

            printf("[0x%X] ", opcode);

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