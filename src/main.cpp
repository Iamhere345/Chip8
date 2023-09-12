#include "chip8.h"
#include <SDL2/SDL.h>

#define app_error(msg, err) { \
    printf("%s (SDL_Error: %s)\n", msg, err); \
    return 1; \
}

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 512;

const int INTERNAL_SCREEN_WIDTH = 64;
const int INTERNAL_SCREEN_HEIGHT = 32;

const int UPSCALE_FACTOR = SCREEN_WIDTH / INTERNAL_SCREEN_WIDTH;      // 16x internal

// TODO use CMake or something

int main(void) {

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


    bool did_load = chip8.load_game("res/ibm.ch8");

    if (!did_load) {
        app_error("Unable to load rom", "N/A");
    }

    //for (int i = 0; i < 4096; i++)
        //printf("%X", chip8.memory[i]);

    // emulation loop
    SDL_Event ev;

    bool active = true;
    while (active) {

        while (SDL_PollEvent(&ev) > 0) {
            switch (ev.type) {
                case SDL_QUIT:
                    active = false;
                    break;
            }
        }
        
        // update chip8
        chip8.cycle();

        // display screen
        for (int i = 0; i < INTERNAL_SCREEN_WIDTH * INTERNAL_SCREEN_HEIGHT; i++) {

            uint8_t pixel = chip8.gfx[i];

            //printf("%d ", i);

            int px = i % INTERNAL_SCREEN_WIDTH;
            int py = i / INTERNAL_SCREEN_WIDTH;

            SDL_Rect screen_pixel = { px * UPSCALE_FACTOR, py * UPSCALE_FACTOR, UPSCALE_FACTOR, UPSCALE_FACTOR };

            switch (pixel) {
                case 0: {
                    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
                    break;
                }
                case 1: {
                    //printf("(%d %d) ", screen_pixel.x, screen_pixel.y);
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

        // wait for the next clock cycle
        SDL_Delay(1000 / 700);

    }


    
    // quit
    SDL_DestroyWindow(window);
    
    SDL_Quit();

    return 0;

}