main: main.o
	gcc main.o -o build/main

main.o: main.c
	gcc -g -c main.c -o main.o