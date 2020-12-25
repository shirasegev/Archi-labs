# All Targets
all: main

main: main.o my_echo.o numbers.o
	gcc -m32 -g -Wall -o main main.o my_echo.o numbers.o

main.o: main.c
	gcc -g -Wall -m32 -c -o main.o main.c

numbers.o: numbers.c
	gcc -g -Wall -m32 -c -o numbers.o numbers.c 
 
add.o: add.s
	nasm -g -f elf -w+all -o my_echo.o my_echo.s


#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o main