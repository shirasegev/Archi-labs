# All Targets
all: task2

task2: task2.o LineParser.o
	gcc -m32 -g -Wall -o task2 task2.o LineParser.o

task2.o: task2.c
	gcc -g -Wall -m32 -c -o task2.o task2.c

LineParser.o: LineParser.c
	gcc -g -Wall -m32 -c -o LineParser.o LineParser.c

#tell make that "clean" is not a file name!
.PHONY: clean


# Check memory leak using valgrind
memleak: all
	valgrind --leak-check=full --show-reachable=yes ./task2

#Clean the build directory
clean: 
	rm -f *.o task2