main.o:	
	gcc -c main.c
piece.o:
	gcc -c piece.c
board.o:
	gcc -c board.c
	
tetris: main.o piece.o board.o
	gcc main.o piece.o board.o -o tetris

clean:
	rm *.o