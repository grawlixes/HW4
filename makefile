test : prog1generator

prog1generator : prog1generator.o commands.o time.o
	gcc -g -Wall prog1generator.o -o prog1generator

prog1generator.o :
	gcc -c prog1generator.c

clean :
	-rm -f *.o prog1generator
