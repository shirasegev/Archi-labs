# All Targets
all: myshell

myshell: myshell.o LineParser.o
	gcc -m32 -g -Wall -o myshell myshell.o LineParser.o 

myshell.o: myshell.c
	gcc -g -Wall -m32 -c -o myshell.o myshell.c

LineParser.o: LineParser.c
	gcc -g -Wall -m32 -c -o LineParser.o LineParser.c

#tell make that "clean" is not a file name!
.PHONY: clean

# Check memory leak using valgrind
memleak: all
	valgrind --leak-check=full --show-reachable=yes ./myshell

#Clean the build directory
clean: 
	rm -f *.o myshell