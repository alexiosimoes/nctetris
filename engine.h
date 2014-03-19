#ifndef _PTETRIS
#define	_PTETRIS
typedef struct board *PtBoard;
typedef struct piece *PtPiece;
PtBoard boardCreate (unsigned width,unsigned height, unsigned initx,unsigned inity,unsigned hidden);	//create a empty board
PtPiece pieceCreate (unsigned width,unsigned height, unsigned rotx, unsigned roty, unsigned rotate);	//create a empty piece
void boardDestroy (PtBoard *board);	//free the space alocated by board
void pieceDestroy (PtPiece *piece);	//free the space alocated by piece
int moveDown (PtBoard board,PtPiece piece);	//move piece down on the board
void moveRight (PtBoard board, PtPiece piece);	//move piece to right
void moveLeft (PtBoard board, PtPiece piece);	//move piece to left
PtPiece pieceRotate (PtBoard board, PtPiece piece);	//rotate piece
int checkLines (PtBoard board, PtPiece piece);	//check and delete complete lines
void pieceSet(PtPiece piece,unsigned x, unsigned y,int value);	//set a point on a specified piece
void boardSet(PtBoard board,unsigned x, unsigned y,int value);	//set a point in the board //for future use
int pieceGet(PtPiece piece,unsigned x, unsigned y);	//get a point in a specified piece
int boardGet(PtBoard board,unsigned x, unsigned y);	//get a point in the board
PtPiece pieceCopy(PtPiece piece);	//returns a copy of a specified piece
int pieceInit(PtBoard board, PtPiece piece);
int pieceWidth(PtPiece piece);
int pieceHeigth(PtPiece piece);
int boardWidth(PtBoard board);
int boardHeigth(PtBoard board);
int boardHidden(PtBoard board);
PtBoard boardClean(PtBoard *board);
#endif


