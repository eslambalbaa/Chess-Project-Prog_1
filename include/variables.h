extern char Wpawn, Wrook, Wknight, Wbishop, Wqueen, Wking;
extern char Bpawn, Brook, Bknight, Bbishop, Bqueen, Bking;
extern char input_move[100];
extern char eaten_piece;
extern char promotion_piece;
extern int undoCount;
typedef struct
{
  char board[8][8];
  int moves_played;
  int white_eaten_count;
  int black_eaten_count;
  char white_eaten[16];
  char black_eaten[16];
  int white_win;
  int black_win;
  int en_pass_col;
  int white_king_moved;
  int white_rook_moved_a;
  int white_rook_moved_h;
  int black_king_moved;
  int black_rook_moved_a;
  int black_rook_moved_h;
} game;
extern game history[1000];
extern game current;