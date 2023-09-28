# Chip8 Emulator

This is a chip8 emulator that i wrote in C++. it's currently a WIP, but most of the instructions are implemented and it can run a few test roms (see `res/`)

# Running Roms

Specify which rom you would like to load as a command line argument
`./chip8 [rom]`

If you don't specify a rom, it will default to `DEFAULT_ROM`

# Building

## On MacOS / Linux

With SDL2 installed, run `make all`. If you get any errors, you may have to change the `LINKER_FLAGS` in the else block.

This has only been tested on MacOS, so i'm not 100% sure if it works on linux, and if it doesn't it should just be a matter of changing the makefile (the build process is very simple so it shouldn't be too hard).

## On Windows

MinGW is required to build on windows.

 - Install MinGW if you have not already
 - Install the MinGW-w64 toolchain (see [this article](https://code.visualstudio.com/docs/cpp/config-mingw#_prerequisites))
 - follow the install instructions for installing SDL2 for MinGW (see [this article](https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/mingw/index.php)).
 - in the Makefile change the windows `INCLUDE_PATHS` and `LIBRARY_PATHS` to your MinGW `include` and `lib` paths.