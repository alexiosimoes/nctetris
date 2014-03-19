#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include "engine.h"

static int level=1,score=0,speed=30;

void defineColors();
void boardDraw(PtBoard board,PtPiece next);   //print the board with colors
PtPiece * loadPieces(char *fname,int *n);
int tetrisMove(PtBoard board,PtPiece *piece,PtPiece next,int speed);

int main()
{
    int i,n; 
    PtBoard board = boardCreate(10,20,5,19,0);
    srand(time(NULL));
    initscr();
    noecho();
    start_color();
    curs_set(0);
    PtPiece * pieces= loadPieces("./pieces",&n);
    PtPiece next,copy=pieceCopy(pieces[rand()%n]);
    while(pieceInit(board,copy))
    {
        next=pieceCopy(pieces[rand()%n]);
        while(tetrisMove(board,&copy,next,speed));
        switch(checkLines(board,copy))
        {
            case 0 :  
                        score+=20;
                        break;
            case 1 :
                        score+=120;
                        break;
            case 2 :    
                        score+=220;
                        break;
            case 3 :    
                        score+=320;
                        break;
            case 4 :    
                        score+=1020;
        }
       if((score/level)>=1000)
        {
            if(++level>10)
                break;
            speed-=3;
            board=boardClean(&board);
        }
        pieceDestroy(&copy);
        copy=next;
        
    }
    for(i=0;i<n;i++)
        pieceDestroy(pieces+i);
    boardDestroy(&board);
    endwin();
    printf("Game Over\n");
    return 0;
}
void defineColors()     //init colors
{
  	init_pair(0,7,COLOR_BLACK);     //board color
	init_pair(1,0,COLOR_RED);       //O color
	init_pair(2,0,COLOR_GREEN);	    //I color
	init_pair(3,0,COLOR_YELLOW);    //S color
	init_pair(4,0,COLOR_BLUE);	    //Z color
    init_pair(5,0,COLOR_MAGENTA);	//L color
	init_pair(6,0,COLOR_CYAN);      //J color
	init_pair(7,0,COLOR_WHITE);     //T color
  
}
void boardDraw(PtBoard board,PtPiece next)   //print the board with colors
{
    int i,j;
    defineColors();
    move(0,0);
    for(i=boardHeigth(board)-1-boardHidden(board);i>=0;i--)              
    {
	    for(j=0;j<boardWidth(board);j++)
		{
			attron(COLOR_PAIR(boardGet(board,j,i)));    //print the game board
            printw("  ");
			attroff(COLOR_PAIR(boardGet(board,j,i)));
		}
                attron(COLOR_PAIR(7));      //print the right border
			printw("|\n");	
			attroff(COLOR_PAIR(7));
    }
		
	attron(COLOR_PAIR(7));              //print the botton border
	for(j=0;j<boardWidth(board)*2+1;j++)
		printw("-");
	attroff(COLOR_PAIR(7));  
    move(0,21);
    printw("LEVEL:%d",level);
    move(1,21);
    printw("SCORE:%d",score);
    move(2,21);
    printw("NEXT PIECE:");
    for(i=0;i<pieceHeigth(next);i++)
    {
        for(j=0;j<pieceWidth(next);j++)
        {
            attron(COLOR_PAIR(pieceGet(next,j,i)));
            move(4-i,23+(j*2));
            printw("  ");
            attroff(COLOR_PAIR(pieceGet(next,j,i)));
        }
    }
    refresh();
    
    
    
}
PtPiece * loadPieces(char * fname,int *n)
{
    FILE *PtF1,*PtF2;
    int value,i,j,k,width,heigth,rotptx,rotpty,rotate;
    char c;
    *n=0;
    
    if ((PtF1=fopen (fname,"r"))==NULL)
    {
        printf("file %s not found",fname);
        return NULL;
    }
    if ((PtF2=fopen (fname,"r"))==NULL)
    {
        printf("file %s not found",fname);
        return NULL;
    }

    while(!feof(PtF1))
    {
        fscanf(PtF1,"%c",&c);
        if(c=='#')
            (*n)++;
    }
    fclose(PtF1);
    PtPiece *piece =(PtPiece *)malloc((*n)*sizeof(PtPiece));
    for(i=0;i<*n;i++)    ///need verifications
    {
        fscanf(PtF2,"#%*s%*c");
        fscanf(PtF2,"width:%d%*c",&width);
        fscanf(PtF2,"heigth:%d%*c",&heigth);
        fscanf(PtF2,"rotationPointx:%d%*c",&rotptx);
        fscanf(PtF2,"rotationPointy:%d%*c",&rotpty);
        fscanf(PtF2,"rotationType:%d%*c",&rotate);
        piece[i]= pieceCreate(width,heigth,rotptx,rotpty,rotate);
        fscanf(PtF2,"array:%*c");
        for(j=pieceHeigth(piece[i])-1;j>=0;j--)
        {
            for(k=0;k<pieceWidth(piece[i]);k++)
            {
                fscanf(PtF2,"%1d",&value);
                if(value)
                    pieceSet(piece[i],k,j,i%7+1);
            }
            fscanf(PtF2,"%*c");    
        } 
    }           
    return piece; 
}
int tetrisMove(PtBoard board,PtPiece *piece,PtPiece next,int speed)
{
    int key,i;
    PtPiece aux;
    keypad(stdscr,TRUE);
    nodelay(stdscr,1);
    for(i=0;i<speed;i++)
        {
            usleep(16666);//60fps
            key=getch();
            switch(key)
            {
                case KEY_UP:	
                        aux=pieceRotate(board,*piece);
					    if(aux!=*piece)
						{
						    pieceDestroy(piece);
							*piece=aux;
						}
					    break;
   	            case KEY_DOWN:
                        if(!moveDown(board,*piece))
                            return 0;
	    				break;
    		    case KEY_LEFT:	
                        moveLeft(board,*piece);
		    			break;
			    case KEY_RIGHT:	
                        moveRight(board,*piece);
				    	break;
			    case 'p':
                    	while(getch()!='p');
					    break;
            }
            boardDraw(board,next);
        }
        if(!moveDown(board,*piece))
                return 0;  
    return 1;
}
