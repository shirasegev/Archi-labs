# All Targets
all: addresses

addresses: addresses.o
	gcc -m32 -g -Wall -o addresses addresses.o

addresses.o: addresses.c
	gcc -g -Wall -m32 -c -o addresses.o addresses.c
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o addresses