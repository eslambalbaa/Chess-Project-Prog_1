#include "variables.h"
#include "criticalscenarios.h"
#include "isitfns.h"
#include "movement.h"
#include "renderfns.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <time.h>
#include "sidenotes.h"
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
    char piece = board[startrow][startcol];
    int rowdiff = destrow - startrow;
    int coldiff = destcol - startcol;
    if( skip == 0 && iswhite(piece) && turn(current.movesplayed)==1){return 0;}
    else if(skip == 0 && isblack(piece) && turn(current.movesplayed)==0){return 0;}
    if(startrow == destrow && startcol == destcol){return 0;}
    if(iswhite(piece) && iswhite(board[destrow][destcol])){return 0;}
    if(isblack(piece) && isblack(board[destrow][destcol])){return 0;}
    switch(piece){
     case 'p':
     if(coldiff == 0){
     if(rowdiff == -1 && isempty(board[destrow][destcol])){return 1;}   
     if(rowdiff == -2 && startrow == 6 && isempty(board[destrow][destcol]) && isempty(board[destrow+1][destcol])){return 1;} 
     }
        else if(abs(coldiff) == 1 && rowdiff == -1) {
        if(!isempty(board[destrow][destcol])){return 1;}
        if(destrow == 2 && destcol == current.enpassCol){return 1;}
    }
     return 0;
     case 'P':
     if(coldiff == 0){
     if(rowdiff == 1 && isempty(board[destrow][destcol])){return 1;}   
     if(rowdiff == 2 && startrow == 1 && isempty(board[destrow][destcol]) && isempty(board[destrow-1][destcol])){return 1;} 
     }
        else if(abs(coldiff) == 1 && rowdiff == 1) {
        if(!isempty(board[destrow][destcol])){return 1;}
        if(destrow == 5 && destcol == current.enpassCol){return 1;}
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
     if((abs(rowdiff) <= 1) && (abs(coldiff) <= 1)){return 1;}
     if(startrow == 7 && rowdiff == 0 &&!current.Wkingmoved && skip==0){
        if(destcol == 6 && ispathclear(board, 7, 7, 7, 4)&& !current.WrookmovedH) {
            if(check(board,0))return 0;
        board[7][5] = 'k';
        board[7][4] = (7+4)%2 ? '.' : '-';
        int f1check = check(board, 0);
        board[7][4] = 'k';
        board[7][5] = (7+5)%2 ? '.' : '-';
        if (f1check) return 0;
        return 1;

        }
        if(destcol == 2 && ispathclear(board, 7, 0, 7, 4) && !current.WrookmovedA) {
            if(check(board,0))return 0;
        board[7][3] = 'k';
        board[7][4] = (7+4)%2 ? '.' : '-';
        int d1check = check(board, 0);
        board[7][4] = 'k';
        board[7][3] = (7+5)%2 ? '.' : '-';
        if (d1check) return 0;
        return 1;
        }
     } 
     return 0;
     case 'K':
     if((abs(rowdiff) <= 1) && (abs(coldiff) <= 1)){return 1;}
     if(startrow == 0 && rowdiff == 0 &&!current.Bkingmoved && skip==0){
        if(destcol == 6 && ispathclear(board, 0, 7, 0, 4)&& !current.BrookmovedH) {
            if(check(board,1))return 0;
            board[0][5] = 'K';
            board[0][4] = (0+4)%2 ? '.' : '-';
            int f8check = check(board, 1);
            board[0][4] = 'K';
            board[0][5] = (0+5)%2 ? '.' : '-';
            if (f8check) return 0;
            return 1;
        }
        if(destcol == 2 && ispathclear(board, 0, 0, 0, 4) && !current.BrookmovedA) {
            if(check(board,1))return 0;
            board[0][3] = 'K';
            board[0][4] = (0+4)%2 ? '.' : '-';
            int d8check = check(board, 1);
            board[0][4] = 'K';
            board[0][3] = (0+5)%2 ? '.' : '-';
            if (d8check) return 0;
            return 1;
        }
     } 
     return 0;
     default:
         return 0;
    }
}


int ispromotion(char board[8][8], char c1, int r1, char c2, int r2){
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