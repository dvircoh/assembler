pro : main.c assembler.h parser.c output.c checks.c secondRead.c initialization.c
	gcc -ansi -pedantic -Wall main.c parser.c output.c checks.c secondRead.c initialization.c -o assembler
