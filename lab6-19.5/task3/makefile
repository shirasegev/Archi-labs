# All Targets
all: mypipeline

mypipeline: mypipeline.o
	gcc -m32 -g -Wall -o mypipeline mypipeline.o

mypipeline.o: mypipeline.c
	gcc -g -Wall -m32 -c -o mypipeline.o mypipeline.c
	
#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o mypipeline