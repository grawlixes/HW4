TARGET1=prog4swap

# Specifiy the target
all: $(TARGET1)

# Specify the object files that the target depends on
# Also specify the object files needed to create the executable
$(TARGET1):	$(TARGET1).o
	gcc $(TARGET1).o -o $(TARGET1) 

# Specify how the object files should be created from source files
$(TARGET1).o:	$(TARGET1).c
	gcc -c -Wall $(TARGET1).c

# Specify the object files and executables that are generated
# and need to be removed to re-compile the whole thing
clean:
	rm -f *.o $(TARGET1)

run:	$(TARGET1)
	./$(TARGET1) -m 5 -r OPT -w No-locality
