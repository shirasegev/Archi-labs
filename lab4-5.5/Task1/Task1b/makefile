# All Targets
all: task1b

task1b: start.o task1b.o util.o
	ld -m elf_i386 start.o task1b.o util.o -o task1b

start.o: start.s
	nasm -f elf start.s -o start.o

task1b.o: task1b.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task1b.c -o task1b.o

util.o: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
 
#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task1b