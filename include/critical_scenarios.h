#ifndef CRITICAL_SCENARIOS_H
#define CRITICAL_SCENARIOS_H

#ifdef __cplusplus
extern "C" {
#endif

int check(char board[8][8], int moves_played);
int checkmate(char board[8][8]);
int stalemate(char board[8][8]);
int endgame(char board[8][8]);
int has_any_legal_move(char board[8][8], int moves_played);

#ifdef __cplusplus
}
#endif

#endif // CRITICAL_SCENARIOS_H
