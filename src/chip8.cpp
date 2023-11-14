#include "chip8.h"
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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
    memset(key, 0, 16);
    memset(memory, 0, 4096);

    // load fontset
    for (int i = 0; i < 80; i++)
        memory[i] = FONTSET[i];

    // reset timers
    delay_timer = 0;
    sound_timer = 0;

    halt = false;
    step = false;

    flag = 0;
    flag_set = false;

    // initialise random number generator
    srand(time(NULL));

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

    fclose(rom);

    printf("**** %d bytes loaded ****\n", i - 0x200);

    return true;

}

bool Chip8::dump_memory(char* path) {

    FILE* mem = fopen(path, "w");

    if (mem == NULL) {
        return false;
    }

    for (int i = 0; i < 4096; i++)
        fputc(memory[i], mem);

    fclose(mem);

    return true;

}

void Chip8::update_keys(SDL_Scancode key_pressed, uint8_t state) {

    switch (key_pressed) {

        case SDL_SCANCODE_1:
            key[0x1] = state;
            break;
        case SDL_SCANCODE_2:
            key[0x2] = state;
            break;
        case SDL_SCANCODE_3:
            key[0x3] = state;
            break;
        case SDL_SCANCODE_4:
            key[0xC] = state;
            break;

        case SDL_SCANCODE_Q:
            key[0x4] = state;
            break;
        case SDL_SCANCODE_W:
            key[0x5] = state;
            break;
        case SDL_SCANCODE_E:
            key[0x6] = state;
            break;
        case SDL_SCANCODE_R:
            key[0xD] = state;
            break;

        case SDL_SCANCODE_A:
            key[0x7] = state;
            break;
        case SDL_SCANCODE_S:
            key[0x8] = state;
            break;
        case SDL_SCANCODE_D:
            key[0x9] = state;
            break;
        case SDL_SCANCODE_F:
            key[0xE] = state;
            break;

        case SDL_SCANCODE_Z:
            key[0xA] = state;
            break;
        case SDL_SCANCODE_X:
            key[0x0] = state;
            break;
        case SDL_SCANCODE_C:
            key[0xB] = state;
            break;
        case SDL_SCANCODE_V:
            key[0xF] = state;
            break;
        
        case SDL_SCANCODE_SPACE:
            if (state) {
                halt = !halt;
            }
            break;
        
        case SDL_SCANCODE_RETURN:
            if (state) {
                step = true;
            }
            break;

        default:
            break;

    }

}

void Chip8::cycle() {

    static bool halt_dump = false;

    if (halt && !step) {
        if (!halt_dump) {
            this->dump_memory("build/memory.bin");
            halt_dump = true;
        }

        return;
    }

    if (step)
        step = false;

    #ifdef CHIP8_DEBUG
    for (int i = 0; i < 32; i++) {
        if (break_addr[i] == pc && !break_active) {
            printf("!!!! breakpoint hit at 0x%X !!!!\n", break_addr[i]);
            halt = true;
            break_active = !break_active;
            return;
        }
    }
    #endif

    // fetch instruction
    opcode = memory[pc] << 8 | memory[pc + 1];

    #ifdef CHIP8_DEBUG
    for (int i = 0; i < 32; i++) {
        if (break_op[i] == opcode && !break_active) {
            printf("!!!! op breakpoint hit on 0x%X @ 0x%X !!!!\n", break_op[i], pc);
            halt = true;
            break_active = !break_active;
            return;
        }
    }
    #endif

    // decode and execute instruction
    bool opcode_executed = false;
    for (int i = 0; i < 35; i++) {

        LOOKUP_INFO info = LOOKUP_TABLE[i];

        if ((opcode & info.mask) == info.opcode) {

            #ifdef CHIP8_DEBUG
            printf("[0x%X][0x%X] ", pc, opcode);
            #endif

            //printf("0x%X & 0x%X == 0x%X\n", opcode, info.mask, info.opcode);

            // i hate this
            (*this.*(info.instruction))(opcode);
            opcode_executed = true;
            break;
        }
    }

    // set VF if the instruction changed flag
    if (flag_set) {
        //printf("[0x%X] set VF to %d\n", opcode, flag);
        V[0xF] = flag;
        flag_set = false;
    }

    if (!opcode_executed) {
        printf("Undefined opcode 0x%X\n", opcode);

        pc += 2;

        halt = true;

    }

}