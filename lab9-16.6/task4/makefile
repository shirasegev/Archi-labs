# All Targets
all: task2b

task2b: task2b.o
	gcc -m32 -g -Wall -o task2b task2b.o

task2b.o: task2b.c
	gcc -g -Wall -m32 -c -o task2b.o task2b.c

.PHONY: clean

# Check memory leak using valgrind
memleak: all
	valgrind --leak-check=full --show-reachable=yes ./task2b ELFexec

#Clean the build directory
clean: 
	rm -f *.o task2b