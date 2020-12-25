# All Targets
all: encoder

encoder: start.o encoder.o util.o
	ld -m elf_i386 start.o encoder.o util.o -o encoder

start.o: start.s
	nasm -f elf start.s -o start.o

encoder.o: encoder.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector encoder.c -o encoder.o

util.o: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
 
#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o encoder