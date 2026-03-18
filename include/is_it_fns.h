int is_valid_move(char board[8][8], char c1, int r1, char c2, int r2, int skip);
int is_path_clear(char board[8][8], int destrow, int destcol, int startrow, int startcol);
int iswhite(char piece);
int isblack(char piece);
int isempty(char place);
int is_promotion(char board[8][8], char c1, int r1, char c2, int r2);
int is_promotion_valid(char board[8][8], char promotion_piece, int startrow, int startcol);