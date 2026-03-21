#include "variables.h"
#include "is_it_fns.h"
#include "movement.h"
#include "critical_scenarios.h"
#include <stdlib.h>
#include <ctype.h>

int iswhite(char piece)
{
    return piece >= 'a' && piece <= 'z';
}

int isblack(char piece)
{
    return piece >= 'A' && piece <= 'Z';
}

int is_path_clear(char board[8][8], int dest_row, int dest_col, int start_row, int start_col)
{
    int row_diff = dest_row - start_row;
    int col_diff = dest_col - start_col;
    int unit_row = 0;
    int unit_col = 0;

    if (col_diff != 0)
        unit_col = col_diff / abs(col_diff);
    if (row_diff != 0)
        unit_row = row_diff / abs(row_diff);

    int current_row = start_row + unit_row;
    int current_col = start_col + unit_col;

    while (current_row != dest_row || current_col != dest_col)
    {
        if (!isempty(board[current_row][current_col]))
            return 0;
        current_row += unit_row;
        current_col += unit_col;
    }
    return 1;
}

int is_valid_move(char board[8][8], char c1, int r1, char c2, int r2, int skip)
{
    if (c1 < 'A' || c1 > 'H' || c2 < 'A' || c2 > 'H')
        return 0;
    if (r1 < 1 || r1 > 8 || r2 < 1 || r2 > 8)
        return 0;

    int dest_col = c2 - 'A';
    int dest_row = 8 - r2;
    int start_col = c1 - 'A';
    int start_row = 8 - r1;
    char piece = board[start_row][start_col];
    int row_diff = dest_row - start_row;
    int col_diff = dest_col - start_col;

    if (skip == 0 && iswhite(piece) && turn(current.moves_played) == 1)
        return 0;
    else if (skip == 0 && isblack(piece) && turn(current.moves_played) == 0)
        return 0;

    if (start_row == dest_row && start_col == dest_col)
        return 0;
    if ((iswhite(piece) && iswhite(board[dest_row][dest_col])) || (isblack(piece) && isblack(board[dest_row][dest_col])))
        return 0;

    switch (piece)
    {
    case 'p':
        if (col_diff == 0)
        {
            if (row_diff == -1 && isempty(board[dest_row][dest_col]))
                return 1;
            if (row_diff == -2 && start_row == 6 && isempty(board[dest_row][dest_col]) && isempty(board[start_row - 1][start_col]))
                return 1;
        }
        else if (abs(col_diff) == 1 && row_diff == -1)
        {
            if (!isempty(board[dest_row][dest_col]))
                return 1;
            if (dest_row == 2 && dest_col == current.en_pass_col)
                return 1;
        }
        return 0;
    case 'P':
        if (col_diff == 0)
        {
            if (row_diff == 1 && isempty(board[dest_row][dest_col]))
                return 1;
            if (row_diff == 2 && start_row == 1 && isempty(board[dest_row][dest_col]) && isempty(board[start_row + 1][start_col]))
                return 1;
        }
        else if (abs(col_diff) == 1 && row_diff == 1)
        {
            if (!isempty(board[dest_row][dest_col]))
                return 1;
            if (dest_row == 5 && dest_col == current.en_pass_col)
                return 1;
        }
        return 0;
    case 'r':
    case 'R':
        return is_path_clear(board, dest_row, dest_col, start_row, start_col) && (row_diff == 0 || col_diff == 0);
    case 'n':
    case 'N':
        return (abs(row_diff) == 2 && abs(col_diff) == 1) || (abs(row_diff) == 1 && abs(col_diff) == 2);
    case 'b':
    case 'B':
        return is_path_clear(board, dest_row, dest_col, start_row, start_col) && abs(row_diff) == abs(col_diff);
    case 'q':
    case 'Q':
        return (row_diff == 0 || col_diff == 0 || abs(row_diff) == abs(col_diff)) && is_path_clear(board, dest_row, dest_col, start_row, start_col);
    case 'k':
        if ((abs(row_diff) <= 1) && (abs(col_diff) <= 1))
            return 1;
        if (start_row == 7 && row_diff == 0 && !current.white_king_moved && skip == 0)
        {
            if (dest_col == 6 && is_path_clear(board, 7, 7, 7, 4) && !current.white_rook_moved_h)
            {
                if (check(board, 0))
                    return 0;
                board[7][5] = 'k';
                board[7][4] = (7 + 4) % 2 ? '.' : '-';
                int f1_check = check(board, 0);
                board[7][4] = 'k';
                board[7][5] = (7 + 5) % 2 ? '.' : '-';
                return !f1_check;
            }
            if (dest_col == 2 && is_path_clear(board, 7, 0, 7, 4) && !current.white_rook_moved_a)
            {
                if (check(board, 0))
                    return 0;
                board[7][3] = 'k';
                board[7][4] = (7 + 4) % 2 ? '.' : '-';
                int d1_check = check(board, 0);
                board[7][4] = 'k';
                board[7][3] = (7 + 3) % 2 ? '.' : '-';
                return !d1_check;
            }
        }
        return 0;
    case 'K':
        if ((abs(row_diff) <= 1) && (abs(col_diff) <= 1))
            return 1;
        if (start_row == 0 && row_diff == 0 && !current.black_king_moved && skip == 0)
        {
            if (dest_col == 6 && is_path_clear(board, 0, 7, 0, 4) && !current.black_rook_moved_h)
            {
                if (check(board, 1))
                    return 0;
                board[0][5] = 'K';
                board[0][4] = (0 + 4) % 2 ? '.' : '-';
                int f8_check = check(board, 1);
                board[0][4] = 'K';
                board[0][5] = (0 + 5) % 2 ? '.' : '-';
                return !f8_check;
            }
            if (dest_col == 2 && is_path_clear(board, 0, 0, 0, 4) && !current.black_rook_moved_a)
            {
                if (check(board, 1))
                    return 0;
                board[0][3] = 'K';
                board[0][4] = (0 + 4) % 2 ? '.' : '-';
                int d8_check = check(board, 1);
                board[0][4] = 'K';
                board[0][3] = (0 + 3) % 2 ? '.' : '-';
                return !d8_check;
            }
        }
        return 0;
    default:
        return 0;
    }
}

int is_promotion(char board[8][8], char c1, int r1, char c2, int r2)
{
    int dest_row = 8 - r2;
    int dest_col = c2 - 'A';
    char piece = board[dest_row][dest_col];

    return (piece == 'p' && iswhite(piece) && dest_row == 0) || (piece == 'P' && isblack(piece) && dest_row == 7);
}

int is_promotion_valid(char board[8][8], char promotion_piece, int start_row, int start_col)
{
    if (board[start_row][start_col] == 'p')
        return promotion_piece == 'b' || promotion_piece == 'n' || promotion_piece == 'q' || promotion_piece == 'r';

    if (board[start_row][start_col] == 'P')
        return promotion_piece == 'B' || promotion_piece == 'N' || promotion_piece == 'Q' || promotion_piece == 'R';

    return 0;
}

int isempty(char place)
{
    return place == '-' || place == '.';
}