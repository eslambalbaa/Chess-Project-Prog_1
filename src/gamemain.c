#include "variables.h"
#include "critical_scenarios.h"
#include "is_it_fns.h"
#include "movement.h"
#include "render_fns.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif
    srand(time(NULL));
    initposition(current.board);
    current.white_king_moved = 0;
    current.white_rook_moved_a = 0;
    current.white_rook_moved_h = 0;
    current.black_king_moved = 0;
    current.black_rook_moved_a = 0;
    current.black_rook_moved_h = 0;
    current.en_pass_col = -1;
    history[0] = current;
    print_board(current.board);
    while (!endgame(current.board))
    {
        movement(current.board);
    }

    return 0;
}
