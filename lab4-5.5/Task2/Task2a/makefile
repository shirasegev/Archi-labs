# All Targets
all: flame2

flame2: start.o flame2.o util.o
	ld -m elf_i386 start.o flame2.o util.o -o flame2

start.o: start.s
	nasm -f elf start.s -o start.o

flame2.o: flame2.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector flame2.c -o flame2.o

util.o: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
 
#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o flame2