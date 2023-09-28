#include "chip8.h"
#include <SDL2/SDL.h>

#define app_error(msg, err) { \
    printf("%s (SDL_Error: %s)\n", msg, err); \
    return 1; \
}

#define DEFAULT_ROM "res/IBM.ch8"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 512;

const int INTERNAL_SCREEN_WIDTH = 64;
const int INTERNAL_SCREEN_HEIGHT = 32;

const int UPSCALE_FACTOR = SCREEN_WIDTH / INTERNAL_SCREEN_WIDTH;      // 16x internal

// clock speed is set to 700Hz
const int EMU_CLOCK_SPEED = 700;

int main(int argc, char** argv) {

    // initialise SDL2
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        app_error("Failed to initialise SDL2", SDL_GetError());
    }

    window = SDL_CreateWindow(
        "Chip8 Emulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) {
        app_error("Failed to initialise renderer", SDL_GetError());
    }

    // create chip8 instance
    Chip8 chip8;

    bool rom_loaded = false;

    if (argc <= 1) {
        printf("#### No rom supplied as arg, defaulting to %s ####\n", DEFAULT_ROM);
        rom_loaded = chip8.load_game(DEFAULT_ROM);
    } else {
        printf("#### loading rom %s ####\n", argv[1]);
        rom_loaded = chip8.load_game(argv[1]);
    }

    if (!rom_loaded) {
        app_error("Unable to load rom", "N/A");
    }

    //for (int i = 0; i < 4096; i++)
        //printf("%X", chip8.memory[i]);

    // timers must be updated 60 times per second (60Hz)
    uint32_t timer_tick = SDL_GetTicks();
    uint32_t timer_interval = 1000 / 60;

    uint32_t chip8_tick = 0;
    uint32_t chip8_interval = 1000 / EMU_CLOCK_SPEED;

    // emulation loop
    SDL_Event ev;

    bool active = true;
    while (active) {

        while (SDL_PollEvent(&ev) > 0) {
            switch (ev.type) {
                case SDL_QUIT:
                    active = false;
                    break;
                
                case SDL_KEYDOWN:
                    chip8.update_keys(ev.key.keysym.scancode);
            }
        }

        // run a cycle and render the screen chip8_interval times per second
        if (SDL_GetTicks() - chip8_tick >= chip8_interval) {

            chip8_tick = SDL_GetTicks();

            // update chip8
            chip8.cycle();

            // refresh keys so when you stop pressing a key it stops being registered
            memset(chip8.key, 0, 16);

            // display screen
            for (int i = 0; i < INTERNAL_SCREEN_WIDTH * INTERNAL_SCREEN_HEIGHT; i++) {

                uint8_t pixel = chip8.gfx[i];

                int px = i % INTERNAL_SCREEN_WIDTH;
                int py = i / INTERNAL_SCREEN_WIDTH;

                SDL_Rect screen_pixel = { px * UPSCALE_FACTOR, py * UPSCALE_FACTOR, UPSCALE_FACTOR, UPSCALE_FACTOR };

                switch (pixel) {
                    case 0: {
                        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
                        break;
                    }
                    case 1: {
                        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                        break;
                    }
                    default: {
                        // malformed pixels are drawn as red for debugging
                        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
                        break;
                    }

                }

                SDL_RenderFillRect(renderer, &screen_pixel);

            }

            SDL_RenderPresent(renderer);
        }

        // tick timers timer_interval times per second
        if (SDL_GetTicks() - timer_tick >= timer_interval) {

            timer_tick = SDL_GetTicks();

            if (chip8.delay_timer > 0) 
                chip8.delay_timer--;

            if (chip8.sound_timer > 0) {
                if (chip8.sound_timer >= 1)
                    printf("BEEP!\n");
                
                chip8.sound_timer--;
            }
        }

        SDL_Delay(1);

    }
    
    // quit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();

    return 0;

}