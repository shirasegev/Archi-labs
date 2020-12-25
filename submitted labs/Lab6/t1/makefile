# All Targets
all: task1

task1: task1.o LineParser.o
	gcc -m32 -g -Wall -o task1 task1.o LineParser.o 

task1.o: task1.c
	gcc -g -Wall -m32 -c -o task1.o task1.c

LineParser.o: LineParser.c
	gcc -g -Wall -m32 -c -o LineParser.o LineParser.c

#tell make that "clean" is not a file name!
.PHONY: clean

# Check memory leak using valgrind
memleak: all
	valgrind --leak-check=full --show-reachable=yes ./task1

#Clean the build directory
clean: 
	rm -f *.o task1
