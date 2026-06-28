#include "variables.h"
#include "critical_scenarios.h"
#include "is_it_fns.h"
#include "movement.h"
#include "render_fns.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_white_turn(int moves_played)
{
    return turn(moves_played) == 0;
}

int is_black_turn(int moves_played)
{
    return turn(moves_played) == 1;
}

static void update_castle_flags(char start_piece, int start_row, int start_col, int dest_col)
{
    if (start_piece == 'k')
        current.white_king_moved = 1;
    else if (start_piece == 'K')
        current.black_king_moved = 1;

    if (start_piece == 'r' && start_row == 7 && start_col == 0)
        current.white_rook_moved_a = 1;
    if (start_piece == 'r' && start_row == 7 && start_col == 7)
        current.white_rook_moved_h = 1;
    if (start_piece == 'R' && start_row == 0 && start_col == 0)
        current.black_rook_moved_a = 1;
    if (start_piece == 'R' && start_row == 0 && start_col == 7)
        current.black_rook_moved_h = 1;

    if ((start_piece == 'k' || start_piece == 'K') && abs(start_col - dest_col) == 2)
    {
        int rook_start_col = (dest_col == 6) ? 7 : 0;
        if (rook_start_col == 7 && start_row == 7)
            current.white_rook_moved_h = 1;
        if (rook_start_col == 0 && start_row == 7)
            current.white_rook_moved_a = 1;
        if (rook_start_col == 7 && start_row == 0)
            current.black_rook_moved_h = 1;
        if (rook_start_col == 0 && start_row == 0)
            current.black_rook_moved_a = 1;
    }
}

int apply_gui_move(char board[8][8], char c1, int r1, char c2, int r2, char prom_piece) {
    if (!is_valid_move(board, c1, r1, c2, r2, 0)) return 0;
    int dest_col = c2 - 'A';
    int dest_row = 8 - r2;
    int start_col = c1 - 'A';
    int start_row = 8 - r1;
    char start_piece = board[start_row][start_col];
    int eaten_piece_val = board[dest_row][dest_col];
    int is_en_passant = 0;
    int en_passant_row = -1;
    
    if ((start_piece == 'p' || start_piece == 'P') && start_col != dest_col && isempty(eaten_piece_val)) {
        is_en_passant = 1;
        en_passant_row = start_row;
        eaten_piece_val = board[en_passant_row][dest_col];
    }
    
    history[current.moves_played] = current;
    undoCount = 0;
    
    board[dest_row][dest_col] = start_piece;
    board[start_row][start_col] = (start_row + start_col) % 2 ? '.' : '-';
    if (is_en_passant) board[en_passant_row][dest_col] = (en_passant_row + dest_col) % 2 ? '.' : '-';
    
    if ((start_piece == 'k' || start_piece == 'K') && abs(start_col - dest_col) == 2) {
        int rook_start_col = (dest_col == 6) ? 7 : 0;
        int rook_dest_col = (dest_col == 6) ? 5 : 3;
        board[start_row][rook_dest_col] = board[start_row][rook_start_col];
        board[start_row][rook_start_col] = (start_row + rook_dest_col) % 2 ? '.' : '-';
    }
    
    update_castle_flags(start_piece, start_row, start_col, dest_col);
    if (!isempty(eaten_piece_val)) eatenpieces(eaten_piece_val);
    
    if (is_promotion(board, c1, r1, c2, r2)) {
            char p = (prom_piece != '\0') ? prom_piece : 'q';
            board[dest_row][dest_col] = (turn(current.moves_played) == 0) ? tolower(p) : toupper(p);
        }
    
    if (check(board, current.moves_played)) {
        current = history[current.moves_played];
        return 0;
    }
    
    current.en_pass_col = -1;
    if (start_piece == 'p' && (dest_row - start_row) == -2) current.en_pass_col = start_col;
    if (start_piece == 'P' && (dest_row - start_row) == 2) current.en_pass_col = start_col;
    current.moves_played++;
    history[current.moves_played] = current;
    return 1;
}

int turn(int moves_played)
{
    return moves_played % 2;
}

void undo()
{
    if (current.moves_played > 0)
    {
        current = history[current.moves_played - 1];
        undoCount++;
        printf("undone successfully\n\n");
    }
    else
        printf("nothing to undo\n\n");
}
void redo()
{
    if (undoCount > 0)
    {
        current = history[current.moves_played + 1];
        undoCount--;
        printf("redone successfully\n\n");
    }
    else
        printf("nothing to redo\n\n");
}

void save_game()
{
    FILE *save = fopen("save.txt", "wb");
    if (save == NULL)
        printf("save failed\n");
    else
    {
        fwrite(&current, sizeof(game), 1, save);
        fwrite(history, sizeof(game), 1000, save);
        fwrite(&undoCount, sizeof(int), 1, save);
        fclose(save);
        printf("saved successfully\n\n");
    }
}
void load_game()
{
    FILE *load = fopen("save.txt", "rb");
    if (load == NULL)
        printf("load failed\n\n");
    else
    {
        fread(&current, sizeof(game), 1, load);
        fread(history, sizeof(game), 1000, load);
        fread(&undoCount, sizeof(int), 1, load);
        fclose(load);
        printf("loaded successfully\n\n");
    }
}
