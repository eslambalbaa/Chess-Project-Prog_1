#include <stdio.h>
#include <string.h>
  char  Wpawn = 'p', Wrook = 'r', Wknight = 'n', Wbishop = 'b', Wqueen = 'q', Wking = 'k';
  char  Bpawn = 'P', Brook = 'R', Bknight = 'N', Bbishop = 'B', Bqueen = 'Q', Bking = 'K';

void printBoard(char board[8][8]);
void initposition(char board[8][8]);
void movement(char board[8][8]);
int isvalidmove(char board[8][8], char c1, int r1, char c2, int r2);
int main(){
   char board[8][8];
   initposition(board);
   printBoard(board);
   while(1){
   movement(board);}

    return 0;
}
int isvalidmove(char board[8][8], char c1, int r1, char c2, int r2){
    return 1;
}
void movement(char board[8][8]){
    char c1, c2;
    int r1, r2;
    printf("Enter move:");
    scanf(" %c%d%c%d", &c1, &r1, &c2, &r2);
    if(isvalidmove(board, c1, r1, c2, r2)){int destcol = c2 - 'A';
    int destrow = 8 - r2;
    int startcol = c1 - 'A';
    int startrow = 8 - r1;
    board[destrow][destcol] = board[startrow][startcol];
    if((startcol+startrow)%2){
        board[startrow][startcol] = '.';
    }
    else{
        board[startrow][startcol] = '-';
    }
    printBoard(board);
    } 
    else{
        printf("Move is invalid, please enter another move\n");
    }
}
void printBoard(char board[8][8]){
    printf("  A B C D E F G H\n");
    for (int i=0;i<8;i++){
        printf("%d ", 8-i);
        for (int j=0;j<8;j++){
            printf("%c ", board[i][j]);}
        printf("%d\n", 8-i);
    }
    printf("  A B C D E F G H\n");
}
void initposition(char board[8][8]){
  char initialboard[8][8] = {{Brook, Bknight, Bbishop, Bqueen, Bking, Bbishop, Bknight, Brook},
                       {Bpawn, Bpawn, Bpawn, Bpawn, Bpawn, Bpawn, Bpawn, Bpawn},
                       {'-', '.', '-', '.', '-', '.', '-', '.'},
                       {'.', '-', '.', '-', '.', '-', '.', '-'},
                       {'-', '.', '-', '.', '-', '.', '-', '.'},
                       {'.', '-', '.', '-', '.', '-', '.', '-'},
                       {Wpawn, Wpawn, Wpawn, Wpawn, Wpawn, Wpawn, Wpawn, Wpawn},
                       {Wrook, Wknight, Wbishop, Wqueen, Wking, Wbishop, Wknight, Wrook}};
    for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      board[i][j] = initialboard[i][j];
    }
}
         }
