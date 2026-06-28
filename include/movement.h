#ifndef MOVEMENT_H
#define MOVEMENT_H

#ifdef __cplusplus
extern "C" {
#endif

int apply_gui_move(char board[8][8], char c1, int r1, char c2, int r2,
                   char prom_piece);
int turn(int moves_played);
int is_white_turn(int moves_played);
int is_black_turn(int moves_played);
void undo();
void redo();
void save_game();
void load_game();

#ifdef __cplusplus
}
#endif

#endif // MOVEMENT_H
