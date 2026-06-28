#include "variables.h"
#include "render_fns.h"
#include "is_it_fns.h"
#include "side_notes.h"
#include <stdio.h>
#include <stdlib.h>

void eatenpieces(int eaten_piece)
{
  if (iswhite(eaten_piece))
    current.white_eaten[current.white_eaten_count++] = eaten_piece;
  else if (isblack(eaten_piece))
    current.black_eaten[current.black_eaten_count++] = eaten_piece;
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
