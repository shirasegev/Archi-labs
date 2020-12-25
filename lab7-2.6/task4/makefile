# All Targets
all: task1 task4

task1: task1.o
	gcc -m32 -g -Wall -o task1 task1.o

task4: task4.o
	gcc -m32 -g -Wall -o task4 task4.o

task1.o: task1.c
	gcc -g -Wall -m32 -fno-pie -c -o task1.o task1.c

task4.o: task4.c
	gcc -g -Wall -m32 -fno-pie -c -o task4.o task4.c
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task1 task4