build/main: main.c proc_win.c proc_posix.c proc.h
	gcc -g main.c proc_win.c proc_posix.c -o build/main