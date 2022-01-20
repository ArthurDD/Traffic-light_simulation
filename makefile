
projet : projet_c.o main.o
	gcc projet_c.o main.o -o projet -lm

projet_c.o : projet_c.h projet_c.c
	gcc -c projet_c.c 

main.o : main.c projet_c.h
	gcc -c main.c



