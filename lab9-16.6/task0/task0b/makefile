# All Targets
all: skeleton

skeleton: skeleton.o
	ld -m elf_i386 skeleton.o -o skeleton

skeleton.o: skeleton.s
	nasm -f elf skeleton.s -o skeleton.o

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o skeleton