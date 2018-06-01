all: cSweeperMain.c mineField.o mineFieldCommandIO.o 
	gcc -g -o cSweeper cSweeperMain.c mineFieldCommandIO.o mineField.o  -std=c99

mineField.o: mineField.c mineField.h 
	gcc -g -c mineField.h mineField.c  -std=c99

mineFieldCommandIO.o: mineFieldCommandIO.h mineFieldCommandIO.c
	gcc -g -c mineFieldCommandIO.h mineFieldCommandIO.c -std=c99

debug: cSweeperMain.c mineField.o
	gcc -g -o debugCSweeper cSweeperMain.c mineField.o -std=c99


clean:
	rm -f core *.o *.gch cSweeper 

