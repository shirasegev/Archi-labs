# All Targets
all: task0

task0: start.o main.o util.o
	ld -m elf_i386 start.o main.o util.o -o task0

start.o: start.s
	nasm -f elf start.s -o start.o

main.o: main.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector main.c -o main.o

util.o: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
 
#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task0