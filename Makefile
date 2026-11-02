build/main: main.o
	gcc build/main.o -o build/main

main.o: main.c
	gcc -g -c main.c -o build/main.o
