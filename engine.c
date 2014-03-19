
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "engine.h"


struct board
{
	unsigned size[2];		//board size, includes the hidden area
	unsigned init[2];		//initial position of the piece
	unsigned actual[2];		//actual position of the piece
	unsigned hidden;		//size of the hidden area on the top of the board
	int **matrix;	
};
struct piece
{
	unsigned size[2];		//piece size
	unsigned rotpoint[2];		//point of rotation
	unsigned rotate;		//type of rotation
	int **matrix;			
};

static int error=0;
static char * errorMsg;
 
static void insert (PtBoard board, PtPiece piece,int x,int y);
static void delete(PtBoard board, PtPiece piece,int x, int y);
static int collition(PtBoard board,PtPiece piece,int x,int y);	//test for colitions
static void startPoint(PtBoard board, PtPiece piece ,int *x,int *y);	//returns by pointer the position 0,0 of the piece on the board

PtBoard boardCreate (unsigned width,unsigned height, unsigned initx,unsigned inity,unsigned hidden)	//create a empty board
{
	assert (width>0);		//verifications
	assert (height>0);		
	assert (initx<width);		
	assert (inity<height);
	assert (hidden<=height);	//
	
	int i;
	PtBoard board;
	
	if((board= (PtBoard) malloc (sizeof(struct board)))==NULL)	//memory allocation to the struct and error handling
		{
			error=1;
			errorMsg="No Memory";					
			return NULL;						
		}
	if((board->matrix = (int **) malloc (width*sizeof(int*)))==NULL)		//memory allocation and error handling
	{
		error=1;
		errorMsg="No Memory";
		free(board);
		return NULL;
	}
	for (i=0;i<width;i++)	
	{
		if((board->matrix[i]=(int *)calloc (height,sizeof(int)))==NULL)	//memory allocation and error handling
		{
			error=1;
			errorMsg="No Memory";
			for (;i>=0;i--)
			{
				free(board->matrix[i]);
			}
			free(board->matrix);
			free(board);
			return NULL;
		}
	}
	board->size[0]=width;			//atribuitions
	board->size[1]=height;
	board->init[0]=board->actual[0]=initx;
	board->init[1]=board->actual[1]=inity;
	board->hidden=hidden;			//
	return board;	
}
PtPiece pieceCreate (unsigned width,unsigned height, unsigned rotx, unsigned roty, unsigned rotate)	//create a empty piece
{
    assert (width>0);		//verifications
	assert (height>0);		
	assert (rotx<width);		
	assert (roty<height);		//
	
	int i;
	PtPiece piece;
	if((piece = (PtPiece) malloc (sizeof(struct piece)))==NULL)	//memory allocation to the struct and error handling
		{
			error=1;
			errorMsg="No Memory";					
			return NULL;						
		}
	if((piece->matrix = (int **) malloc (width*sizeof(int*)))==NULL)	//memory allocation of pointers array and error handling
	{
		error=1;
		errorMsg="No Memory";
		free(piece);
		return NULL;
	}
	for (i=0;i<width;i++)	
	{
		if((piece->matrix[i]=(int *)calloc (height,sizeof(int)))==NULL)	//memory allocation of bidimentional array and error handling
		{
			error=1;
			errorMsg="No Memory";
			for (;i>=0;i--)
				free(piece->matrix[i]);
			free(piece->matrix);
			free(piece);
			return NULL;
		}
	}
	piece->size[0]=width;		//atribuitions
	piece->size[1]=height;
	piece->rotpoint[0]=rotx;
	piece->rotpoint[1]=roty;
	piece->rotate=rotate;
	return piece;			//

}
void boardDestroy (PtBoard *board)	//free the space alocated by board
{
	assert (board!=NULL);		//verifications	
	assert (*board!=NULL);
	int i;
	for (i=0; i<(*board)->size[0]; i++)
		free((*board)->matrix[i]);//free of array	
	free((*board)->matrix);		//free of pointers array
	free(*board);			//free of struct
	*board=NULL;
}
void pieceDestroy (PtPiece *piece)	//free the space alocated by piece
{
	assert (piece!=NULL);
	assert (*piece!=NULL);
	int i;
	for (i=0; i<(*piece)->size[0]; i++)
		free((*piece)->matrix[i]);	//free of array	
	free((*piece)->matrix);			//free of pointers array
	free(*piece);				//free of struct
	*piece=NULL;
}
PtBoard boardClean(PtBoard *board)
{
	PtBoard new= boardCreate((*board)->size[0],(*board)->size[1],(*board)->init[0],(*board)->init[1],(*board)->hidden);
    boardDestroy(board);
    return new;
    
}
int moveDown (PtBoard board,PtPiece piece)	//move piece down on the board
{
	assert (board!=NULL);
	assert (piece!=NULL);
	int x,y;
	startPoint(board,piece,&x,&y);		//get the 0,0 position
	delete(board,piece,x,y);		//delete piece
	if(collition(board,piece,x,y-1))		//collition check
	{
		insert(board,piece,x,y);		
		return 0;
	}
	insert(board,piece,x,y-1);		//insert piece in the new position		
	board->actual[1]--;
	return 1;	
			
}
void moveRight (PtBoard board, PtPiece piece)	//move piece to right
{
	assert (board!=NULL);
	assert (piece!=NULL);
	int x,y;
	startPoint(board,piece,&x,&y);		//get the 0,0 position
	delete(board,piece,x,y);		//delete piece
	if(collition(board,piece,x+1,y))		//collition check
	{
		insert(board,piece,x,y);		
		return;
	}
	insert(board,piece,x+1,y);		//insert piece in the new position		
	board->actual[0]++;

}
void moveLeft (PtBoard board, PtPiece piece)	//move piece to left
{
	assert (board!=NULL);
	assert (piece!=NULL);
	int x,y;
	startPoint(board,piece,&x,&y);		//get the 0,0 position
	delete(board,piece,x,y);		//delete piece
	if(collition(board,piece,x-1,y))		//collition check
	{
		insert(board,piece,x,y);		
		return ;
	}
	insert(board,piece,x-1,y);		//insert piece in the new position		
	board->actual[0]--;

}
PtPiece pieceRotate (PtBoard board, PtPiece piece)	//rotate piece
{
	assert (board!=NULL);
	assert (piece!=NULL);
	assert (piece->rotate<3);
	int nx,ny,x,y,i,j;
	PtPiece	new;

	startPoint(board,piece,&x,&y);		//get the 0,0 position
	delete(board,piece,x,y);		//delete piece
	
	
	switch(piece->rotate)
	{
		case 0 :	new = pieceCreate(piece->size[1],piece->size[0],piece->rotpoint[1],piece->rotpoint[0],piece->rotate);
				break;
		case 1 :	new = pieceCreate(piece->size[1],piece->size[0],piece->size[1]-piece->rotpoint[1]-1,piece->size[0]-piece->rotpoint[0]-1,piece->rotate);
				break;
		case 2 :	new = pieceCreate(piece->size[1],piece->size[0],piece->size[1]-piece->rotpoint[1]-1,piece->rotpoint[0],piece->rotate);
				break;	
	}

	for(i=0;i<piece->size[0];i++)
		for(j=0;j<piece->size[1];j++)
				new->matrix[piece->size[1]-j-1][i]=piece->matrix[i][j];
		

	startPoint(board,new,&nx,&ny);		//get the 0,0 position
	if(collition(board,new,nx,ny))		//collition check
	{
		insert(board,piece,x,y);		
		return piece;
	}

	insert(board,new,nx,ny);		//insert piece in the new position					
	return new;
	
	
	
}


int checkLines (PtBoard board, PtPiece piece)	//check and delete complete lines
{
	int r=0,i,j,l,k,x,y,flag;
	startPoint(board,piece,&x,&y);
	for(i=y;i-y<piece->size[1];i++)
	{
		flag=0;
		for(j=0;j<board->size[0];j++)
			if(!board->matrix[j][i])
			{
				flag=1;
				break;
			}
		if(!flag)
		{
			for(k=i;k<board->size[1]-1;k++)
				for(l=0;l<board->size[0];l++)
					board->matrix[l][k]=board->matrix[l][k+1];
			r++;
			i--;
			
		}	
	}
			
	return r;		
	
	
}
void pieceSet(PtPiece piece,unsigned x, unsigned y,int value)	//set a point on a specified piece
{
	assert(piece!=NULL);
	assert(x<piece->size[0]&& y<piece->size[1]);
	piece->matrix[x][y]=value;
}
void boardSet(PtBoard board,unsigned x, unsigned y,int value)	//set a point in the board ///for future use
{
	assert(board!=NULL);
	assert(x<board->size[0]&& y<board->size[1]);
	board->matrix[x][y]=value;
}

int pieceGet(PtPiece piece,unsigned x, unsigned y)	//get a point in a specified piece
{
	assert(piece!=NULL);
	assert(x<piece->size[0]&& y<piece->size[1]);
	return piece->matrix[x][y];
}
int boardGet(PtBoard board,unsigned x, unsigned y)	//get a point in the board
{
	assert(board!=NULL);
	assert(x<board->size[0]&& y<board->size[1]);
	return board->matrix[x][y];
}

PtPiece pieceCopy(PtPiece piece)	//returns a copy of a specified piece
{
	int i,j;
	assert (piece!=NULL);
	PtPiece new = pieceCreate(piece->size[0],piece->size[1],piece->rotpoint[0],piece->rotpoint[1],piece->rotate);
	for(i=0;i<piece->size[0];i++)
		for(j=0;j<piece->size[1];j++)
			new->matrix[i][j]=piece->matrix[i][j];
	return new;
}
int pieceInit(PtBoard board, PtPiece piece) //initializate the piece on top of the board
{
	assert(board!=NULL);
	assert(piece!=NULL);
	if(collition(board,piece,board->init[0],board->init[1]))
		return 0;
	board->actual[0]=board->init[0];
	board->actual[1]=board->init[1];
	
	int x,y;
	startPoint(board,piece,&x,&y);

	insert(board,piece,x,y);
	return 1;	
}
int pieceWidth(PtPiece piece)
{
	assert(piece!=NULL);
	return piece->size[0];
} 
int pieceHeigth(PtPiece piece)
{
	assert(piece!=NULL);
	return piece->size[1];
} 
int boardWidth(PtBoard board)
{
	assert(board!=NULL);
	return board->size[0];
} 
int boardHeigth(PtBoard board)
{
	assert(board!=NULL);
	return board->size[1];
} 
int boardHidden(PtBoard board)
{
	assert(board!=NULL);
	return board->hidden;
}
static void insert (PtBoard board, PtPiece piece,int x, int y)	//insert piece on the boad
{
	int i,j,k,l;
	for (i=0,k=x;i<piece->size[0];i++,k++)
		for(j=0,l=y;j<piece->size[1];j++,l++)
			if(piece->matrix[i][j])
				board->matrix[k][l]=piece->matrix[i][j];

}
static void delete(PtBoard board, PtPiece piece,int x, int y)	//delete piece from the board
{ 

	int i,j,k,l;
	for (i=0,k=x;i<piece->size[0];i++,k++)
		for(j=0,l=y;j<piece->size[1];j++,l++)
			if(piece->matrix[i][j])
				board->matrix[k][l]=0;
}
static int collition(PtBoard board,PtPiece piece,int x,int y)	//test for colitions
{
	if(x<0 || y<0 || x+piece->size[0]>board->size[0] || y+piece->size[1]-1>board->size[1])	//test for collition with the borders///it works i dont know why
		return 1; 
	int i,j,k,l;
	for (i=0,k=x;i<piece->size[0];i++,k++)							//test for overlaping collitions
		for(j=0,l=y;j<piece->size[1];j++,l++)					
			if(piece->matrix[i][j] && board->matrix[k][l])
				return 1;						//
	return 0;
}
static void startPoint(PtBoard board, PtPiece piece ,int *x,int *y)	//returns by pointer the position 0,0 of the piece on the board
{
	*x= board->actual[0]-piece->rotpoint[0];
	*y= board->actual[1]-piece->rotpoint[1];
}
