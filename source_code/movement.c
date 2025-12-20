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


void movement(char board[8][8]){
    if(check(board, current.movesplayed)){printf("Check!\n");}
    char c1, c2;
    int r1, r2;
      if(turn(current.movesplayed)==0){
       printf("White's move:");
    }
    else{
        printf("Black's move:");
    }

    fgets(inputmove, 80, stdin);
    if (strchr(inputmove, '\n') == NULL){clearinputbuffer();}
    cleaninput(inputmove);

    if(strcmp(inputmove, "UNDO") == 0){
        if(current.movesplayed > 0){
        undo();
        printBoard(current.board);
        undoCount++;
        }
        else{
            printf("nothing to undo\n\n");
        }
        return;
    }

    if(strcmp(inputmove, "REDO") == 0){
        if(undoCount > 0){
            redo();
            printBoard(current.board);
            undoCount--;
        } else {
            printf("nothing to redo\n\n");
        }
        return;
    }
    if(strcmp(inputmove, "SAVE") == 0){
        Save(); 
        return;
    }
    if(strcmp(inputmove, "LOAD") == 0){
        Load();
        printBoard(current.board);
        return;
    }
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
    int isenpass = 0;
    int enpassR = -1;
    if((startpiece == 'p' || startpiece == 'P') && startcol != destcol && isempty(eatenpiece)){
        isenpass = 1;
        enpassR = startrow; 
        eatenpiece = board[enpassR][destcol];
        }
        history[current.movesplayed] = current;
        board[destrow][destcol] = startpiece;
        if(isenpass){
            board[enpassR][destcol] = (enpassR + destcol) % 2 ? '.' : '-';
        }
    if(ispromotion(board, c1, r1, c2, r2)){
        while(1){
        printf("Promotion What would you like to upgrade to?\nbishop(B or b), knight(N or n), queen(Q or q), rook(R or r):");
         char inputprom[10];
         fgets(inputprom, 10, stdin);
         if (strchr(inputprom, '\n') == NULL){clearinputbuffer();}
         cleaninput(inputprom);
         promotionpiece = inputprom[0];
         if(turn(current.movesplayed) == 0){promotionpiece = tolower(promotionpiece);}
         else if(turn(current.movesplayed) == 1){promotionpiece = toupper(promotionpiece);}
         if(!ispromotionvalid(board, promotionpiece, startrow, startcol)){
        printf("Promotion invalid please enter another one\n");}
        else{board[destrow][destcol] = promotionpiece;
        break;}  
}}
    if(check(board, current.movesplayed)){
        printf("Illegal move it puts your king in check\n");
            board[startrow][startcol] = startpiece;
            if(isenpass){
                board[destrow][destcol] = (destrow + destcol) % 2 ? '.' : '-';
                board[enpassR][destcol] = eatenpiece;
            }
            else{
                board[destrow][destcol] = eatenpiece;
            }
        return;
    }
    board[startrow][startcol] = (startcol+startrow)%2 ? '.' : '-';
    if(eatenpiece != '-' && eatenpiece != '.'){eatenpieces(eatenpiece);}
    current.enpassCol = -1;
    if(startpiece == 'p' && (destrow - startrow) == -2) { current.enpassCol = startcol; }
    if(startpiece == 'P' && (destrow - startrow) == 2) { current.enpassCol = startcol; }
    current.movesplayed++;
    undoCount = 0;
    history[current.movesplayed] = current;
    printBoard(board);
    
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
     if(input[i] != ' ' && input[i] != '\n' && input[i] != '\r'){
   input[j] = toupper(input[i]);
   j++;}
}
input[j] = '\0';
}


int turn(int movesplayed){
    if(movesplayed%2==0){
        return 0;
    }
        return 1;
}


void undo(){
    if(current.movesplayed>0){
        current=history[current.movesplayed-1];
        printf("undone successfully\n\n");
    }
}
    void redo(){
        current=history[current.movesplayed+1];
        printf("redone successfully\n\n");
    }

void Save(){
    FILE *save= fopen("save.txt", "wb");
    if(save==NULL)
        printf("save failed\n");
        else{
            fwrite(&current, sizeof(game), 1, save);
            fclose(save);
            printf("saved successfully\n\n");
        }
}
void Load(){
    FILE *load= fopen("save.txt", "rb");
    if(load==NULL)
        printf("load failed\n\n");
        else{
            fread(&current, sizeof(game), 1, load);
            fclose(load);
            printf("loaded successfully\n\n");
        }
}