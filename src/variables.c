#include "variables.h"

char Wpawn = 'p', Wrook = 'r', Wknight = 'n', Wbishop = 'b', Wqueen = 'q', Wking = 'k';
char Bpawn = 'P', Brook = 'R', Bknight = 'N', Bbishop = 'B', Bqueen = 'Q', Bking = 'K';
char input_move[100];
char eaten_piece;
char promotion_piece;
int undoCount = 0;
game history[1000];
game current;