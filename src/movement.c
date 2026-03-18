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

static void update_castle_flags(char startpiece, int startrow, int startcol, int destcol)
{
    if (startpiece == 'k')
    {
        current.white_king_moved = 1;
    }
    else if (startpiece == 'K')
    {
        current.black_king_moved = 1;
    }
    if (startpiece == 'r' && startrow == 7 && startcol == 0)
    {
        current.white_rook_moved_a = 1;
    }
    if (startpiece == 'r' && startrow == 7 && startcol == 7)
    {
        current.white_rook_moved_h = 1;
    }
    if (startpiece == 'R' && startrow == 0 && startcol == 0)
    {
        current.black_rook_moved_a = 1;
    }
    if (startpiece == 'R' && startrow == 0 && startcol == 7)
    {
        current.black_rook_moved_h = 1;
    }
    if ((startpiece == 'k' || startpiece == 'K') && abs(startcol - destcol) == 2)
    {
        int rokstartcol = (destcol == 6) ? 7 : 0;
        if (rokstartcol == 7 && startrow == 7)
            current.white_rook_moved_h = 1;
        if (rokstartcol == 0 && startrow == 7)
            current.white_rook_moved_a = 1;
        if (rokstartcol == 7 && startrow == 0)
            current.black_rook_moved_h = 1;
        if (rokstartcol == 0 && startrow == 0)
            current.black_rook_moved_a = 1;
    }
}

void movement(char board[8][8])
{
    if (check(board, current.moves_played))
    {
        printf("Check!\n");
    }
    char c1, c2;
    int r1, r2;
    if (is_white_turn(current.moves_played))
    {
        printf("White's move:");
    }
    else
    {
        printf("Black's move:");
    }

    fgets(input_move, 80, stdin);
    if (strchr(input_move, '\n') == NULL)
    {
        clear_input_buffer();
    }
    clean_input(input_move);

    if (strcmp(input_move, "UNDO") == 0)
    {
        if (current.moves_played > 0)
        {
            undo();
            print_board(current.board);
            undoCount++;
        }
        else
        {
            printf("nothing to undo\n\n");
        }
        return;
    }

    if (strcmp(input_move, "REDO") == 0)
    {
        if (undoCount > 0)
        {
            redo();
            print_board(current.board);
            undoCount--;
        }
        else
        {
            printf("nothing to redo\n\n");
        }
        return;
    }
    if (strcmp(input_move, "SAVE") == 0)
    {
        save_game();
        return;
    }
    if (strcmp(input_move, "LOAD") == 0)
    {
        load_game();
        print_board(current.board);
        return;
    }
    if (strcmp(input_move, "QUIT") == 0)
    {
        exit(0);
        return;
    }
    if (strlen(input_move) != 4)
    {
        printf("Move is invalid, please enter another move\n");
        return;
    }
    c1 = input_move[0];
    c2 = input_move[2];
    r1 = input_move[1] - '0';
    r2 = input_move[3] - '0';
    if (is_valid_move(board, c1, r1, c2, r2, 0))
    {
        int destcol = c2 - 'A';
        int destrow = 8 - r2;
        int startcol = c1 - 'A';
        int startrow = 8 - r1;
        int eaten_piece = board[destrow][destcol];
        char startpiece = board[startrow][startcol];
        int isenpass = 0;
        int enpassR = -1;
        if ((startpiece == 'p' || startpiece == 'P') && startcol != destcol && isempty(eaten_piece))
        {
            isenpass = 1;
            enpassR = startrow;
            eaten_piece = board[enpassR][destcol];
        }
        history[current.moves_played] = current;
        board[destrow][destcol] = startpiece;
        board[startrow][startcol] = (startrow + startcol) % 2 ? '.' : '-';
        if (isenpass)
        {
            board[enpassR][destcol] = (enpassR + destcol) % 2 ? '.' : '-';
        }
        if ((startpiece == 'k' || startpiece == 'K') && abs(startcol - destcol) == 2)
        {
            int rokstartcol = (destcol == 6) ? 7 : 0;
            int rokdestcol = (destcol == 6) ? 5 : 3;
            board[startrow][rokdestcol] = board[startrow][rokstartcol];
            board[startrow][rokstartcol] = (startrow + rokdestcol) % 2 ? '.' : '-';
        }

        update_castle_flags(startpiece, startrow, startcol, destcol);
        if (is_promotion(board, c1, r1, c2, r2))
        {
            while (1)
            {
                printf("Promotion What would you like to upgrade to?\nbishop(B or b), knight(N or n), queen(Q or q), rook(R or r):");
                char inputprom[10];
                fgets(inputprom, 10, stdin);
                if (strchr(inputprom, '\n') == NULL)
                {
                    clear_input_buffer();
                }
                clean_input(inputprom);
                promotion_piece = inputprom[0];
                if (turn(current.moves_played) == 0)
                {
                    promotion_piece = tolower(promotion_piece);
                }
                else if (turn(current.moves_played) == 1)
                {
                    promotion_piece = toupper(promotion_piece);
                }
                if (!is_promotion_valid(board, promotion_piece, destrow, destcol) || (strlen(inputprom) != 1))
                {
                    printf("Promotion invalid please enter another one\n");
                }
                else
                {
                    board[destrow][destcol] = promotion_piece;
                    break;
                }
            }
        }
        if (check(board, current.moves_played))
        {
            printf("Illegal move it puts your king in check\n");
            current = history[current.moves_played];
            return;
        }
        if (eaten_piece != '-' && eaten_piece != '.')
        {
            eatenpieces(eaten_piece);
        }
        current.en_pass_col = -1;
        if (startpiece == 'p' && (destrow - startrow) == -2)
        {
            current.en_pass_col = startcol;
        }
        if (startpiece == 'P' && (destrow - startrow) == 2)
        {
            current.en_pass_col = startcol;
        }
        current.moves_played++;
        undoCount = 0;
        history[current.moves_played] = current;
        print_board(board);
    }
    else
    {
        printf("Move is invalid, please enter another move\n");
    }
}

void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

void clean_input(char *input)
{
    int j = 0;
    for (int i = 0; input[i] != '\0'; i++)
    {
        if (input[i] != ' ' && input[i] != '\n' && input[i] != '\r')
        {
            input[j] = toupper(input[i]);
            j++;
        }
    }
    input[j] = '\0';
}

int turn(int moves_played)
{
    if (moves_played % 2 == 0)
    {
        return 0;
    }
    return 1;
}

void undo()
{
    if (current.moves_played > 0)
    {
        current = history[current.moves_played - 1];
        printf("undone successfully\n\n");
    }
}
void redo()
{
    current = history[current.moves_played + 1];
    printf("redone successfully\n\n");
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