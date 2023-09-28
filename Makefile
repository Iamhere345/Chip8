ifeq ($(OS),Windows_NT)
	#OBJS specifies which files to compile as part of the project
	OBJS = src\main.cpp src\chip8.cpp src\instructions.cpp

	#CC specifies which compiler we're using
	CC = g++

	#INCLUDE_PATHS specifies the additional include paths we'll need
	INCLUDE_PATHS = -ID:\dev\mingw_libs\x86_64-w64-mingw32\include -Iinclude

	#LIBRARY_PATHS specifies the additional library paths we'll need
	LIBRARY_PATHS = -LD:\dev\mingw_libs\x86_64-w64-mingw32\lib

	#COMPILER_FLAGS specifies the additional compilation options we're using
	# -w suppresses all warnings
	# -Wl,-subsystem,windows gets rid of the console window
	COMPILER_FLAGS = -Wall #-Wl,-subsystem,windows

	#LINKER_FLAGS specifies the libraries we're linking against
	LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2

	#OBJ_NAME specifies the name of our exectuable
	OBJ_NAME = build\chip8
else
	OBJS = src/main.cpp src/chip8.cpp src/instructions.cpp

	CC = g++

	INCLUDE_PATHS = -Iinclude

	COMPILER_FLAGS = -Wall

	LINKER_FLAGS = -lsdl2

	OBJ_NAME = build/chip8
endif

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)