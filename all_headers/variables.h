  extern char  Wpawn , Wrook  , Wknight  , Wbishop , Wqueen , Wking ;
  extern char  Bpawn , Brook , Bknight , Bbishop , Bqueen , Bking;
  extern char inputmove[100];
  extern char eatenpiece;
  extern char promotionpiece;
  typedef struct{
    char board[8][8];
    int movesplayed;
    int whiteeatencount;
    int blackeatencount;
    char whiteeaten[16];
    char blackeaten[16];
    int whitewin;
    int blackwin;
  } game;
  extern game history[1000];
  extern game current;