#include "variables.h"
#include "render_fns.h"
#include "is_it_fns.h"
#include "side_notes.h"
#include <stdio.h>

void print_board(char board[8][8])
{
  printf("    A   B   C   D   E   F   G   H\n");
  printf("  +---+---+---+---+---+---+---+---+\n");
  for (int i = 0; i < 8; i++)
  {
    printf("%d |", 8 - i);
    for (int j = 0; j < 8; j++)
    {
      printf(" %s |", piecechangeforprint(board[i][j]));
    }
    if (i == 1)
    {
      printf(" %d ", 8 - i);
      printf("\t\tWhite taken out:");
      for (int r = 0; r < current.white_eaten_count; r++)
      {
        if (r == current.white_eaten_count - 1)
        {
          printf("%s", piecechangeforprint(current.white_eaten[r]));
        }
        else
        {
          printf("%s, ", piecechangeforprint(current.white_eaten[r]));
        }
      }
      printf("\n  +---+---+---+---+---+---+---+---+\n");
    }
    else if (i == 2)
    {
      printf(" %d", 8 - i);
      printf("\t\tBlack taken out:");
      for (int s = 0; s < current.black_eaten_count; s++)
      {
        if (s == current.black_eaten_count - 1)
        {
          printf("%s", piecechangeforprint(current.black_eaten[s]));
        }
        else
        {
          printf("%s, ", piecechangeforprint(current.black_eaten[s]));
        }
      }
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
    {
      printf(" %d\n  +---+---+---+---+---+---+---+---+\n", 8 - i);
    }
  }
  printf("    A   B   C   D   E   F   G   H\n");
}

void initposition(char board[8][8])
{
  char initialboard[8][8] = {{Brook, Bknight, Bbishop, Bqueen, Bking, Bbishop, Bknight, Brook},
                             {Bpawn, Bpawn, Bpawn, Bpawn, Bpawn, Bpawn, Bpawn, Bpawn},
                             {'-', '.', '-', '.', '-', '.', '-', '.'},
                             {'.', '-', '.', '-', '.', '-', '.', '-'},
                             {'-', '.', '-', '.', '-', '.', '-', '.'},
                             {'.', '-', '.', '-', '.', '-', '.', '-'},
                             {Wpawn, Wpawn, Wpawn, Wpawn, Wpawn, Wpawn, Wpawn, Wpawn},
                             {Wrook, Wknight, Wbishop, Wqueen, Wking, Wbishop, Wknight, Wrook}};
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      board[i][j] = initialboard[i][j];
    }
  }
}

void eatenpieces(int eaten_piece)
{
  if (iswhite(eaten_piece))
  {
    current.white_eaten[current.white_eaten_count] = eaten_piece;
    current.white_eaten_count++;
  }
  else if (isblack(eaten_piece))
  {
    current.black_eaten[current.black_eaten_count] = eaten_piece;
    current.black_eaten_count++;
  }
}

char *piecechangeforprint(char piece)
{
  switch (piece)
  {
  case 'p':
    return "♙";
    break;
  case 'P':
    return "♟";
    break;
  case 'r':
    return "♖";
    break;
  case 'R':
    return "♜";
    break;
  case 'n':
    return "♘";
    break;
  case 'N':
    return "♞";
    break;
  case 'b':
    return "♗";
    break;
  case 'B':
    return "♝";
    break;
  case 'q':
    return "♕";
    break;
  case 'Q':
    return "♛";
    break;
  case 'k':
    return "♔";
    break;
  case 'K':
    return "♚";
    break;
  case '.':
    return ".";
    break;
  case '-':
    return "-";
    break;
  default:
    break;
  }
  return "";
}