# All Targets
all: task0a

task0a: task0a.o
	gcc -m32 -g -Wall -o task0a task0a.o

task0a.o: task0a.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task0a.c -o task0a.o
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task0a