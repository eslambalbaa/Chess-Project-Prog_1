#include <stdio.h>
#include <string.h>
#include <stdlib.h>
  char  Wpawn = 'p', Wrook = 'r', Wknight = 'n', Wbishop = 'b', Wqueen = 'q', Wking = 'k';
  char  Bpawn = 'P', Brook = 'R', Bknight = 'N', Bbishop = 'B', Bqueen = 'Q', Bking = 'K';
  int movesplayed = 0;

void printBoard(char board[8][8]);
void initposition(char board[8][8]);
void movement(char board[8][8]);
int isvalidmove(char board[8][8], char c1, int r1, char c2, int r2);
int checkmate(char board[8][8], char c1, int r1, char c2, int r2);
int turn(int movesplayed);
int isempty(char place);
void eatenpieces(int eatenpiece);
int iswhite(char piece);
int isblack(char piece);
int ispathclear(char board[8][8], int destrow, int destcol, int startrow, int startcol);
int endgame(char board[8][8]);


int main(){
   char board[8][8];
   initposition(board);
   printBoard(board);
   while(!endgame(board)){
   movement(board);}

    return 0;
}


int iswhite(char piece){
    if(piece == Wpawn || piece == Wrook || piece == Wknight || piece == Wbishop || piece == Wqueen || piece == Wking){
        return 1;
    }
    else{
        return 0;
    }
}
int isblack(char piece){
    if(piece == Bpawn || piece == Brook || piece == Bknight || piece == Bbishop || piece == Bqueen || piece == Bking){
        return 1;
    }
    else{
        return 0;
    }    
}


int isempty(char place){
 if((place=='-') || (place == '.'))
    {return 1;}
 else
    {return 0;}
}


void movement(char board[8][8]){
    char c1, c2;
    int r1, r2;
      if(turn(movesplayed)==0){
       printf("White's move:");
    }
    else{
        printf("Black's move:");
    }
    scanf(" %c%d%c%d", &c1, &r1, &c2, &r2);
    if(isvalidmove(board, c1, r1, c2, r2)){
    int destcol = c2 - 'A';
    int destrow = 8 - r2;
    int startcol = c1 - 'A';
    int startrow = 8 - r1;
    int eatenpiece = board[destrow][destcol];
    board[destrow][destcol] = board[startrow][startcol];
    if((startcol+startrow)%2){
        board[startrow][startcol] = '.';
    }
    else{
        board[startrow][startcol] = '-';
    }
    printBoard(board);
    if(eatenpiece != '-' && eatenpiece != '.'){eatenpieces(eatenpiece);}
    movesplayed++;
    if(checkmate(board, c1, r1, c2, r2)){
        printf("Chckmate!\n");
    } }
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


int checkmate(char board[8][8], char c1, int r1, char c2, int r2){
        return 0;
}


int turn(int movesplayed){
    if(movesplayed%2==0){
        return 0;
    }
    else if(movesplayed%2==1){
        return 1;
    }
}


int ispathclear(char board[8][8], int destrow, int destcol, int startrow, int startcol){
    int rowdiff = destrow - startrow;
    int coldiff = destcol - startcol;
    int unitrow = 0;
    int unitcol = 0;
    if(coldiff != 0){
        unitcol = coldiff/abs(coldiff);
    }
    if(rowdiff != 0){
        unitrow = rowdiff/abs(rowdiff);
    }
    int currentrow = startrow + unitrow;
    int currentcol = startcol + unitcol;
    while(currentrow != destrow || currentcol != destcol){
         if(!isempty(board[currentrow][currentcol])){
             return 0;
         }
         currentrow += unitrow;
         currentcol += unitcol;
    }
     return 1;
}


int isvalidmove(char board[8][8], char c1, int r1, char c2, int r2){
    if (c1< 'A' || c1>'H' || c2< 'A' || c2>'H') {return 0;}
    if (r1< 1 || r1>8 || r2<1 || r2>8) {return 0;}
    int destcol = c2 - 'A';
    int destrow = 8 - r2;
    int startcol = c1 - 'A';
    int startrow = 8 - r1;
    int whoseturn;
    char piece = board[startrow][startcol];
    int rowdiff = destrow - startrow;
    int coldiff = destcol - startcol;
    if(turn(movesplayed)==0){whoseturn = 0;}
    else if(turn(movesplayed)==1){whoseturn =1;}
    if(iswhite(piece) && turn(movesplayed)==1){return 0;}
    else if(isblack(piece) && turn(movesplayed)==0){return 0;}
    if(startrow == destrow && startcol == destcol){return 0;}
    if(iswhite(piece) && iswhite(board[destrow][destcol])){return 0;}
    if(isblack(piece) && isblack(board[destrow][destcol])){return 0;}
    switch(piece){
     case 'p':
     if(coldiff == 0){
     if(rowdiff == -1 && isempty(board[destrow][destcol])){return 1;}   
     if(rowdiff == -2 && startrow == 6 && isempty(board[destrow][destcol]) && isempty(board[destrow+1][destcol])){return 1;} 
     }
     else if(abs(coldiff) == 1){
         if(rowdiff == -1 && !isempty(board[destrow][destcol])){return 1;}
     }
     return 0;    
     case 'P':
     if(coldiff == 0){
     if(rowdiff == 1 && isempty(board[destrow][destcol])){return 1;}   
     if(rowdiff == 2 && startrow == 1 && isempty(board[destrow][destcol]) && isempty(board[destrow-1][destcol])){return 1;} 
     }
     else if(abs(coldiff) == 1){
         if(rowdiff == 1 && !isempty(board[destrow][destcol])){return 1;}
     }
     return 0;  
     case 'r':
     case 'R':
     if(ispathclear(board, destrow, destcol, startrow, startcol) && (rowdiff == 0 || coldiff == 0)){return 1;}
     return 0;
     case 'n':
     case 'N':
     if((abs(rowdiff) == 2 && abs(coldiff) == 1) || (abs(rowdiff) == 1 && abs(coldiff) == 2)){return 1;}
     return 0;
     case 'b':
     case 'B':
     if(ispathclear(board, destrow, destcol, startrow, startcol) && abs(rowdiff) == abs(coldiff)){return 1;}
     return 0;
     case 'q':
     case 'Q':
     if((rowdiff == 0 || coldiff == 0 || abs(rowdiff) == abs(coldiff)) && ispathclear(board, destrow, destcol, startrow, startcol)){return 1;}
     return 0;
     case 'k':
     case 'K':
     if((abs(rowdiff) <= 1) && (abs(coldiff) <= 1)){return 1;}
     return 0;
     default:
         return 0;
    }
}


void eatenpieces(int eatenpiece){
    printf("Eaten piece: %c\n", eatenpiece);
}


int endgame(char board[8][8]){
    int checkK1 = 0;
    int checkK2 = 0;
     for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (board[i][j] == 'K'){
                checkK2 = 1;
            }
            else if (board[i][j] == 'k'){
                checkK1 = 1;
            }
        }
    }
    if(checkK1 == 1 && checkK2 == 1){
        return 0;}
    else if(checkK1 == 1 && checkK2 == 0){
        printf("White won!");
    }
    else if(checkK2 == 1 && checkK1 == 0){
        printf("Black won!");
    }
    return 1;
}