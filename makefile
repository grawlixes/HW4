test : prog1generator

prog1generator : prog1generator.o commands.o time.o
	gcc -g -Wall 80-20.o -o 80-20

prog1generator.o :
	gcc -c 80-20.c

clean :
	-rm -f *.o 80-20
