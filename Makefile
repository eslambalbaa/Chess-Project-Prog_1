CC = gcc
CXX = g++

SRC_DIR = src
INC_DIR = include
SDL_DIR = vendor/SDL2

CFLAGS = -Wall -g -I$(INC_DIR) -I$(SDL_DIR)/include
CXXFLAGS = -Wall -g -I$(INC_DIR) -I$(SDL_DIR)/include

LDFLAGS = -L$(SDL_DIR)/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

# Added gui.o here
OBJS = main.o gui.o variables.o movement.o render_fns.o is_it_fns.o critical_scenarios.o side_notes.o

Target = chessgame.exe

all: $(Target)

$(Target): $(OBJS)
	$(CXX) -o $(Target) $(OBJS) $(LDFLAGS)
	@echo Removing object files...
	@del /q $(OBJS)

main.o: $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

gui.o: $(SRC_DIR)/gui.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /q *.o $(Target)
