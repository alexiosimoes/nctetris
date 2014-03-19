CC=gcc
CFLAGS= -Wall -lncurses

OBJM=engine.o
HEADERS=engine.h

OBJECTS=$(OBJM) tetris.o
EXES=nctetris

all: $(EXES)

nctetris:	tetris.o $(OBJM)
			$(CC) $(CFLAGS) tetris.o $(OBJM) -o nctetris

$(OBJM):	$(HEADERS)

clean:
			rm -f $(OBJECTS) *~

cleanall:	clean
			rm -f $(EXES)
