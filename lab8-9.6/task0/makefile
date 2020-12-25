# All Targets
all: myELF

myELF: myELF.o
	gcc -m32 -g -Wall -o myELF myELF.o

myELF.o: myELF.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector myELF.c -o myELF.o
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o myELF