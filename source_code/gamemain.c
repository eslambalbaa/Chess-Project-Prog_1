#include "variables.h"
#include "criticalscenarios.h"
#include "isitfns.h"
#include "movement.h"
#include "renderfns.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <time.h>
#include "sidenotes.h"

int main(){
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    #endif
    srand(time(NULL));
    initposition(current.board);
    current.Wkingmoved=0;
    current.WrookmovedA=0;
    current.WrookmovedH=0;
    current.Bkingmoved=0;
    current.BrookmovedA=0;
    current.BrookmovedH=0;
    current.enpassCol = -1;
    history[0] = current;
    printBoard(current.board);
    while(!endgame(current.board)){
    movement(current.board);}

    return 0;
}
