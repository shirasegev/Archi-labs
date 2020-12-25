# All Targets
all: mypipe

mypipe: mypipe.o LineParser.o
	gcc -m32 -g -Wall -o mypipe mypipe.o LineParser.o 

mypipe.o: mypipe.c
	gcc -g -Wall -m32 -c -o mypipe.o mypipe.c

LineParser.o: LineParser.c
	gcc -g -Wall -m32 -c -o LineParser.o LineParser.c

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o mypipe