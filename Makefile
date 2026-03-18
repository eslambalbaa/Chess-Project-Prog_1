CC = gcc
SRC_DIR = src
INC_DIR = include
CFLAGS = -Wall -g -I$(INC_DIR)

OBJS = gamemain.o variables.o movement.o render_fns.o is_it_fns.o critical_scenarios.o side_notes.o

Target = chessgame

all: $(Target)

$(Target): $(OBJS)
	$(CC) $(CFLAGS) -o $(Target) $(OBJS)
	@echo Removing object files...
	@del /q $(OBJS)

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /q *.o $(Target).exe