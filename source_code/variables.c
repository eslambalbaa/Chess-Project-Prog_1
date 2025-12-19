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
  char  Wpawn = 'p', Wrook = 'r', Wknight = 'n', Wbishop = 'b', Wqueen = 'q', Wking = 'k';
  char  Bpawn = 'P', Brook = 'R', Bknight = 'N', Bbishop = 'B', Bqueen = 'Q', Bking = 'K';
  char inputmove[100];
  char eatenpiece;
  char promotionpiece;
  game history[1000];
  game current;