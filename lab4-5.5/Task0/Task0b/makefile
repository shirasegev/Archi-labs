# All Targets
all: patch

patch: start.o patch.o util.o
	ld -m elf_i386 start.o patch.o util.o -o patch

start.o: start.s
	nasm -f elf start.s -o start.o

patch.o: patch.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector patch.c -o patch.o

util.o: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
 
#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o patch