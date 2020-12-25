# All Targets
all: encoder

encode: encoder.o
	gcc -m32 -g -Wall -o encoder encoder.o

encode.o: encoder.c
	gcc -g -Wall -m32 -c -o encoder.o encoder.c

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o encoder