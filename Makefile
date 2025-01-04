index:	index.o
	gcc -ansi -Wall index.o -o index -lm
index.o:	index.c
	gcc -c -ansi -Wall -pedantic index.c -lm
