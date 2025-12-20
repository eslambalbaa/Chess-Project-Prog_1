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

int check(char board[8][8], int movesplayed){
  int Krow = -1, Kcol = -1;
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


int checkmate(char board[8][8]){
    char temp;
    if(!check(board, current.movesplayed)){return 0;}
    for(int i =0 ; i < 8; i++){
        for(int j = 0; j < 8; j++){
        if(iswhite(board[i][j]) && turn(current.movesplayed) == 1){continue;}
        else if(isblack(board[i][j]) && turn(current.movesplayed) == 0){continue;}
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
    if(!check(board, current.movesplayed)){
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
 if(turn(current.movesplayed) == 1){current.whitewin = 1;}
 else if(turn(current.movesplayed) == 0){current.blackwin = 1;}  
    return 1;
}


int stalemate(char board[8][8]){
    char temp;
    if(check(board, current.movesplayed)){return 0;}
    for(int i =0 ; i < 8; i++){
        for(int j = 0; j < 8; j++){
      if(iswhite(board[i][j]) && turn(current.movesplayed) == 1){continue;}
        else if(isblack(board[i][j]) && turn(current.movesplayed) == 0){continue;}
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
    if(!check(board, current.movesplayed)){
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
        if(current.whitewin == 1){
            printf("White won! Checkmate");
            return 1;}
        else if(current.blackwin == 1){
            printf("Black won! Checkmate");
            return 1;}
        }
    else if(stalemate(board)){
        printf("Stalemate!");
        return 1;}
        return 0;
}
