# All Targets
all: task1 task2 task3

task1: task1.o
	gcc -m32 -g -Wall -o task1 task1.o

task1.o: task1.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task1.c -o task1.o

task2: task2.o
	gcc -m32 -g -Wall -o task2 task2.o

task2.o: task2.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task2.c -o task2.o

task3: task3.o
	gcc -m32 -g -Wall -o task3 task3.o

task3.o: task3.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task3.c -o task3.o

.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task1 task2 task3