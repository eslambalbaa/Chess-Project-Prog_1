#include "variables.h"
#include "critical_scenarios.h"
#include "is_it_fns.h"
#include "movement.h"
#include <stdio.h>

int check(char board[8][8], int moves_played)
{
    int Krow = -1, Kcol = -1;
    char undercheck, King;
    if (turn(moves_played) == 0)
    {
        King = 'k';
    }
    else if (turn(moves_played) == 1)
    {
        King = 'K';
    }
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j] == King)
            {
                Krow = i;
                Kcol = j;
            }
        }
    }
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            undercheck = board[i][j];
            if (isempty(undercheck))
            {
                continue;
            }

            if (iswhite(undercheck) && is_black_turn(moves_played))
            {
                if (is_valid_move(board, j + 'A', 8 - i, Kcol + 'A', 8 - Krow, 1))
                {
                    return 1;
                }
            }
            else if (isblack(undercheck) && is_white_turn(moves_played))
            {
                if (is_valid_move(board, j + 'A', 8 - i, Kcol + 'A', 8 - Krow, 1))
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

static int has_any_legal_move(char board[8][8], int moves_played)
{
    char temp;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            char piece = board[i][j];
            if (isempty(piece))
            {
                continue;
            }
            if ((iswhite(piece) && is_black_turn(moves_played)) || (isblack(piece) && is_white_turn(moves_played)))
            {
                continue;
            }
            for (int r = 0; r < 8; r++)
            {
                for (int c = 0; c < 8; c++)
                {
                    if (is_valid_move(board, j + 'A', 8 - i, c + 'A', 8 - r, 1))
                    {
                        temp = board[r][c];
                        board[r][c] = board[i][j];
                        board[i][j] = ((i + j) % 2) ? '.' : '-';
                        if (!check(board, moves_played))
                        {
                            board[i][j] = board[r][c];
                            board[r][c] = temp;
                            return 1;
                        }
                        board[i][j] = board[r][c];
                        board[r][c] = temp;
                    }
                }
            }
        }
    }
    return 0;
}

int checkmate(char board[8][8])
{
    if (!check(board, current.moves_played))
    {
        return 0;
    }
    if (has_any_legal_move(board, current.moves_played))
    {
        return 0;
    }

    if (is_black_turn(current.moves_played))
    {
        current.white_win = 1;
    }
    else
    {
        current.black_win = 1;
    }
    return 1;
}

int stalemate(char board[8][8])
{
    if (check(board, current.moves_played))
    {
        return 0;
    }
    return !has_any_legal_move(board, current.moves_played);
}

int endgame(char board[8][8])
{
    if (checkmate(board))
    {
        if (current.white_win == 1)
        {
            printf("White won! Checkmate");
            return 1;
        }
        else if (current.black_win == 1)
        {
            printf("Black won! Checkmate");
            return 1;
        }
    }
    else if (stalemate(board))
    {
        printf("Stalemate!");
        return 1;
    }
    return 0;
}
