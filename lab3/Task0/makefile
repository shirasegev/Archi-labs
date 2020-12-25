# All Targets
all: bubblesort hexaPrint

hexaPrint: hexaPrint.o
	gcc -m32 -g -Wall -o hexaPrint hexaPrint.o

hexaPrint.o: hexaPrint.c
	gcc -g -Wall -m32 -c -o hexaPrint.o hexaPrint.c

bubblesort: bubblesort.o
	gcc -m32 -g -Wall -o bubblesort bubblesort.o

bubblesort.o: bubblesort.c
	gcc -g -Wall -m32 -c -o bubblesort.o bubblesort.c

.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o bubblesort hexaPrint