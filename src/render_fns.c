#include "variables.h"
#include "render_fns.h"
#include "is_it_fns.h"
#include "side_notes.h"
#include <stdio.h>
#include <stdlib.h>

void clear_screen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void print_board(char board[8][8])
{
  clear_screen();
  printf("    A   B   C   D   E   F   G   H\n");
  printf("  +---+---+---+---+---+---+---+---+\n");
  for (int i = 0; i < 8; i++)
  {
    printf("%d |", 8 - i);
    for (int j = 0; j < 8; j++)
      printf(" %s |", piecechangeforprint(board[i][j]));

    if (i == 1)
    {
      printf(" %d ", 8 - i);
      printf("\t\tWhite taken out:");
      for (int r = 0; r < current.white_eaten_count; r++)
        printf("%s%s", piecechangeforprint(current.white_eaten[r]), (r == current.white_eaten_count - 1) ? "" : ", ");
      printf("\n  +---+---+---+---+---+---+---+---+\n");
    }
    else if (i == 2)
    {
      printf(" %d", 8 - i);
      printf("\t\tBlack taken out:");
      for (int s = 0; s < current.black_eaten_count; s++)
        printf("%s%s", piecechangeforprint(current.black_eaten[s]), (s == current.black_eaten_count - 1) ? "" : ", ");
      printf("\n  +---+---+---+---+---+---+---+---+\n");
    }
    else if (i == 0)
    {
      printf(" %d", 8 - i);
      printf("\t\t");
      sidenote();
      printf("\n  +---+---+---+---+---+---+---+---+\n");
    }
    else
      printf(" %d\n  +---+---+---+---+---+---+---+---+\n", 8 - i);
  }
  printf("    A   B   C   D   E   F   G   H\n");
}

void eatenpieces(int eaten_piece)
{
  if (iswhite(eaten_piece))
    current.white_eaten[current.white_eaten_count++] = eaten_piece;
  else if (isblack(eaten_piece))
    current.black_eaten[current.black_eaten_count++] = eaten_piece;
}

char *piecechangeforprint(char piece)
{
  switch (piece)
  {
  case 'p':
    return "♙";
  case 'P':
    return "♟";
  case 'r':
    return "♖";
  case 'R':
    return "♜";
  case 'n':
    return "♘";
  case 'N':
    return "♞";
  case 'b':
    return "♗";
  case 'B':
    return "♝";
  case 'q':
    return "♕";
  case 'Q':
    return "♛";
  case 'k':
    return "♔";
  case 'K':
    return "♚";
  case '.':
    return ".";
  case '-':
    return "-";
  default:
    return "";
  }
}

void initposition(char board[8][8])
{
  char initial_board[8][8] = {
      {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
      {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
      {'-', '.', '-', '.', '-', '.', '-', '.'},
      {'.', '-', '.', '-', '.', '-', '.', '-'},
      {'-', '.', '-', '.', '-', '.', '-', '.'},
      {'.', '-', '.', '-', '.', '-', '.', '-'},
      {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
      {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}};

  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      board[i][j] = initial_board[i][j];
}