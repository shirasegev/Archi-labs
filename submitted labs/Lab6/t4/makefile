# All Targets
all: task4

task4: task4.o LineParser.o
	gcc -m32 -g -Wall -o task4 task4.o LineParser.o 

task4.o: task4.c
	gcc -g -Wall -m32 -c -o task4.o task4.c

LineParser.o: LineParser.c
	gcc -g -Wall -m32 -c -o LineParser.o LineParser.c

#tell make that "clean" is not a file name!
.PHONY: clean

# Check memory leak using valgrind
memleak: all
	valgrind --leak-check=full --show-reachable=yes ./task4

#Clean the build directory
clean: 
	rm -f *.o task4
