#ifndef _NCTETRIS
#define	_NCTETRIS
typedef struct board *PtBoard;
typedef struct piece *PtPiece;
PtBoard createBoard (unsigned width,unsigned height, unsigned initx,unsigned inity,unsigned hidden);	//create a empty board
PtPiece createPiece (unsigned width,unsigned height, unsigned rotx, unsigned roty, unsigned rotate);	//create a empty piece
void destroyBoard (PtBoard *board);	//free the space alocated by board
void destroyPiece (PtPiece *piece);	//free the space alocated by piece
int moveDown (PtBoard board,PtPiece piece);	//move piece down on the board
void moveRight (PtBoard board, PtPiece piece);	//move piece to right
void moveLeft (PtBoard board, PtPiece piece);	//move piece to left
PtPiece pieceRotate (PtBoard board, PtPiece piece);	//rotate piece
int checkLines (PtBoard board, PtPiece piece);	//check and delete complete lines
void setPiece(PtPiece piece,unsigned x, unsigned y,int value);	//set a point on a specified piece
void setBoard(PtBoard board,unsigned x, unsigned y,int value);	//set a point in the board //for future use
int getPiece(PtPiece piece,unsigned x, unsigned y);	//get a point in a specified piece
int getBoard(PtBoard board,unsigned x, unsigned y);	//get a point in the board
PtPiece copyPiece(PtPiece piece);	//returns a copy of a specified piece
int initPiece(PtBoard board, PtPiece piece);
int pieceWidth(PtPiece piece);
int pieceHeigth(PtPiece piece);
int boardWidth(PtBoard board);
int boardHeigth(PtBoard board);
int boardHidden(PtBoard board);
#endif


