# All Targets
all: hexeditplus

hexeditplus: hexeditplus.o
	gcc -m32 -g -Wall -o hexeditplus hexeditplus.o

hexeditplus.o: hexeditplus.c
	gcc -g -Wall -m32 -c -o hexeditplus.o hexeditplus.c
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o hexeditplus