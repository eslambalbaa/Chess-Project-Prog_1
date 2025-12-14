#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <time.h>
  char  Wpawn = 'p', Wrook = 'r', Wknight = 'n', Wbishop = 'b', Wqueen = 'q', Wking = 'k';
  char  Bpawn = 'P', Brook = 'R', Bknight = 'N', Bbishop = 'B', Bqueen = 'Q', Bking = 'K';
  int movesplayed = 0;
  char whiteeaten[16], blackeaten[16], inputmove[100];
  int whiteeatencount = 0, blackeatencount = 0;
  char eatenpiece;
  int whitewin = 0, blackwin = 0;
  char promotionpiece;


void printBoard(char board[8][8]);
void initposition(char board[8][8]);
void movement(char board[8][8]);
int isvalidmove(char board[8][8], char c1, int r1, char c2, int r2, int skip);
int check(char board[8][8], int movesplayed);
int checkmate(char board[8][8]);
int turn(int movesplayed);
int isempty(char place);
void eatenpieces(int eatenpiece);
int iswhite(char piece);
int isblack(char piece);
int ispathclear(char board[8][8], int destrow, int destcol, int startrow, int startcol);
int endgame(char board[8][8]);
int stalemate(char board[8][8]);
int ispromotion(char board[8][8], char c1, int r1, char c2, int r2);
int ispromotionvalid(char board[8][8],char promotionpiece , int  startrow, int startcol);
char* piecechangeforprint(char piece);
void sidenote();
void clearinputbuffer();
void cleaninput(char* input);




int main(){
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    #endif
   srand(time(NULL));
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
    if(check(board, movesplayed)){printf("Check!\n");}
    char c1, c2;
    int r1, r2;
      if(turn(movesplayed)==0){
       printf("White's move:");
    }
    else{
        printf("Black's move:");
    }
    fgets(inputmove, 80, stdin);
    if (strchr(inputmove, '\n') == NULL){clearinputbuffer();}
    cleaninput(inputmove);
    if(strlen(inputmove) != 4){printf("Move is invalid, please enter another move\n"); return;}
    c1 = inputmove[0];
    c2 = inputmove[2];
    r1 = inputmove[1] - '0';
    r2 = inputmove[3] - '0';    
    if(isvalidmove(board, c1, r1, c2, r2, 0)){
    int destcol = c2 - 'A';
    int destrow = 8 - r2;
    int startcol = c1 - 'A';
    int startrow = 8 - r1;
    int eatenpiece = board[destrow][destcol];
    char startpiece = board[startrow][startcol];
    board[destrow][destcol] = board[startrow][startcol];
    if(ispromotion(board, c1, r1, c2, r2)){
        while(1){
        printf("Promotion What would you like to upgrade to?\nbishop(B or b), knight(N or n), queen(Q or q), rook(R or r):");
         char inputprom[10];
         fgets(inputprom, 10, stdin);
         if (strchr(inputprom, '\n') == NULL){clearinputbuffer();}
         cleaninput(inputprom);
         promotionpiece = inputprom[0];
         if(turn(movesplayed) == 0){promotionpiece = tolower(promotionpiece);}
         else if(turn(movesplayed) == 1){promotionpiece = toupper(promotionpiece);}
         if(!ispromotionvalid(board, promotionpiece, startrow, startcol)){
        printf("Promotion invalid please enter another one\n");}
        else{board[destrow][destcol] = promotionpiece;
        break;}  
}}
    if(check(board, movesplayed)){
        printf("Illegal move it puts your king in check\n");
        board[startrow][startcol] = startpiece;
        board[destrow][destcol] = eatenpiece;
        return;
    }
    if((startcol+startrow)%2){
        board[startrow][startcol] = '.';
    }
    else{
        board[startrow][startcol] = '-';
    }
    if(eatenpiece != '-' && eatenpiece != '.'){eatenpieces(eatenpiece);}
    printBoard(board);
    movesplayed++;
    }
    else{
        printf("Move is invalid, please enter another move\n");
    }
}


void clearinputbuffer(){
    int c;
    while(( c = getchar()) != '\n' && c != EOF){}
}


void cleaninput(char* input){
    int j = 0;
    for(int i = 0; input[i] != '\0' ; i++){
     if(input[i] != ' ' && input[i] != '\n'){
   input[j] = toupper(input[i]);
   j++;}
}
input[j] = '\0';
}


void printBoard(char board[8][8]){
    printf("    A   B   C   D   E   F   G   H\n");
    printf("  +---+---+---+---+---+---+---+---+\n");
    for (int i=0;i<8;i++){
        printf("%d |", 8-i);
        for (int j=0;j<8;j++){
            printf(" %s |", piecechangeforprint(board[i][j]));}
        if(i == 1){
            printf(" %d ", 8-i); 
            printf("\t\tWhite taken out:"); 
            for(int r=0;r<whiteeatencount;r++){if(r==whiteeatencount-1){printf("%s", piecechangeforprint(whiteeaten[r]));} else{printf("%s, ", piecechangeforprint(whiteeaten[r]));}}
            printf("\n  +---+---+---+---+---+---+---+---+\n");}
        else if(i == 2){
            printf(" %d", 8-i); 
            printf("\t\tBlack taken out:");  
            for(int s=0;s<blackeatencount;s++){if(s==blackeatencount-1){printf("%s", piecechangeforprint(blackeaten[s]));} else{printf("%s, ", piecechangeforprint(blackeaten[s]));}}
            printf("\n  +---+---+---+---+---+---+---+---+\n");}
        else if(i == 0){printf(" %d", 8-i);
            printf("\t\t"); sidenote();
            printf("\n  +---+---+---+---+---+---+---+---+\n");}
            
        else{printf(" %d\n  +---+---+---+---+---+---+---+---+\n", 8-i);}
    }
    printf("    A   B   C   D   E   F   G   H\n");
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


int isvalidmove(char board[8][8], char c1, int r1, char c2, int r2, int skip){
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
    if(skip == 0 && turn(movesplayed)==0){whoseturn = 0;}
    else if(skip == 0 && turn(movesplayed)==1){whoseturn =1;}
    if( skip == 0 && iswhite(piece) && turn(movesplayed)==1){return 0;}
    else if(skip == 0 && isblack(piece) && turn(movesplayed)==0){return 0;}
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
    if(iswhite(eatenpiece)){
        whiteeaten[whiteeatencount] = eatenpiece;
        whiteeatencount++;
    }
    else if(isblack(eatenpiece)){
       blackeaten[blackeatencount] = eatenpiece;
        blackeatencount++;
    }
}


int check(char board[8][8], int movesplayed){
  int Krow, Kcol;
  char undercheck, King;
  if(turn(movesplayed)==0){
       King = 'k';}
  else if(turn(movesplayed)==1){
       King = 'K';
  }
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
        if(board[i][j] == King){
            Krow = i;
            Kcol = j;}}
        }
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            undercheck = board[i][j];
            if(isempty(undercheck)){continue;}

        if(iswhite(undercheck) && turn(movesplayed) == 1){
            if(isvalidmove(board, j+'A', 8-i, Kcol+'A', 8-Krow, 1)){return 1;}
        }
        else if(isblack(undercheck) && turn(movesplayed) == 0){
            if(isvalidmove(board, j+'A', 8-i, Kcol+'A', 8-Krow, 1)){return 1;}
        }
    }}
    return 0;    
        }


int ispromotion(char board[8][8], char c1, int r1, char c2, int r2){
    int destcol = c2 - 'A';
    int destrow = 8 - r2;
    int startcol = c1 - 'A';
    int startrow = 8 - r1;
    char piece = board[startrow][startcol];
    if(piece == 'p' && iswhite(piece) && destrow == 0){return 1;}
    else if(piece == 'P' && isblack(piece) && destrow == 7){return 1;}
    return 0;
}


int ispromotionvalid(char board[8][8], char promotionpiece,int  startrow, int startcol){
     if(board[startrow][startcol] == 'p'){
        if ( promotionpiece == 'b' || promotionpiece == 'n' || promotionpiece == 'q' || promotionpiece == 'r'){return 1;}}
     else if(board[startrow][startcol] == 'P'){
         if ( promotionpiece == 'B' || promotionpiece == 'N' || promotionpiece == 'Q' || promotionpiece == 'R'){return 1;}
     }
 return 0;
}


int checkmate(char board[8][8]){
    char temp;
    if(!check(board, movesplayed)){return 0;}
    for(int i =0 ; i < 8; i++){
        for(int j = 0; j < 8; j++){
        if(iswhite(board[i][j]) && turn(movesplayed) == 1){continue;}
        else if(isblack(board[i][j]) && turn(movesplayed) == 0){continue;}
        else if(isempty(board[i][j])){continue;}
        for (int r = 0; r < 8; r++){
            for (int c = 0; c < 8; c++){
                if(isvalidmove(board, j+'A', 8-i, c+'A', 8-r, 1)){
             temp = board[r][c];
            board[r][c] = board[i][j];
            if((i+j)%2){
        board[i][j] = '.';
    }
    else{
        board[i][j] = '-';
    }
    if(!check(board, movesplayed)){
        board[i][j] = board[r][c];
        board[r][c] = temp;
        return 0;}
        board[i][j] = board[r][c];
        board[r][c] = temp;
    }
    
    }
}
        }
    }
 if(turn(movesplayed) == 1){whitewin = 1;}
 else if(turn(movesplayed) == 0){blackwin = 1;}  
    return 1;
}


int stalemate(char board[8][8]){
    char temp;
    if(check(board, movesplayed)){return 0;}
    for(int i =0 ; i < 8; i++){
        for(int j = 0; j < 8; j++){
      if(iswhite(board[i][j]) && turn(movesplayed) == 1){continue;}
        else if(isblack(board[i][j]) && turn(movesplayed) == 0){continue;}
        else if(isempty(board[i][j])){continue;}
        for (int r = 0; r < 8; r++){
            for (int c = 0; c < 8; c++){
     if(isvalidmove(board, j+'A', 8-i, c+'A', 8-r, 1)){
             temp = board[r][c];
             board[r][c] = board[i][j];
             if((i+j)%2){
             board[i][j] = '.';
    }
    else{
             board[i][j] = '-';
    }
    if(!check(board, movesplayed)){
        board[i][j] = board[r][c];
        board[r][c] = temp;
        return 0;}
        board[i][j] = board[r][c];
        board[r][c] = temp;
    }
}   
}
        }
    }
    return 1;
}

int endgame(char board[8][8]){
    if(checkmate(board)){
        if(whitewin == 1){
            printf("White won! Checkmate");
            return 1;}
        else if(blackwin == 1){
            printf("Black won! Checkmate");
            return 1;}
        }
    else if(stalemate(board)){
        printf("Stalemate!");
        return 1;}
        return 0;
}


char* piecechangeforprint(char piece){
   switch(piece){
   case 'p': return "♙";
     break;
   case 'P': return "♟";
     break;
   case 'r': return "♖";
     break;
   case 'R': return "♜";
     break;
   case 'n': return "♘";
     break;
   case 'N': return "♞";
     break;
   case 'b': return "♗";
     break;
   case 'B': return "♝";
     break;
   case 'q': return "♕";
     break;
   case 'Q': return "♛";
     break;
   case 'k': return "♔";
     break;
   case 'K': return "♚";
     break;
   case '.': return ".";
    break;
   case '-': return "-";
    break;
   default: break;
   } 
}


void sidenote(){
  int qoute = rand()%13;
  switch(qoute){
    case 0:
    printf("2 is forever lonely in the prime world!");
    break;
    case 1:
    printf("Error 404: Motivation Not Found");
    break;
    case 2:
    printf("One man's constant is another man's variable");
    break;
    case 3:
    printf("while(alive){ code; }");
    break;
    case 4:
    printf("1 + 1= 10 (but in binary!)");
    break;
    case 5:
    printf("Just keep taking chances and having fun");
    break;
    case 6:
    printf("!false (it's true!)");
    break;
    case 7:
    printf("Recursion: see 'Recursion'");
    break;
    case 8:
    printf("It always seems impossible until it's done");
    break;
    case 9:
    printf("Real Programmers Count From 0");
    break;
    case 10:
    printf("Decimals have a point.");
    break;
    case 11:
    printf("π: Be Rational,i: Get Real");
    break;
    case 12:
    printf("It's not a bug, it's a feature");
    break;
    default:
    break;
  }
}