# All Targets
all: looper

looper: looper.o 
	gcc -m32 -g -Wall -o looper looper.o

looper.o: looper.c
	gcc -g -Wall -m32 -c -o looper.o looper.c


#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o looper