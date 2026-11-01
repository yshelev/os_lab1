build/main.exe: main.o
	gcc build/main.o -o build/main.exe

main.o: main.c
	gcc -g -c main.c -o build/main.o
