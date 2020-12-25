# All Targets
all: detectAndFixViruses
# Tool invocations
# Executable "detectAndFixViruses" depends on the file detectAndFixViruses.c.
detectAndFixViruses: detectAndFixViruses.o
	gcc -g -m32 -Wall -o detectAndFixViruses detectAndFixViruses.o

# Depends on the source and header files
detectAndFixViruses.o: detectAndFixViruses.c
	gcc -m32 -g -Wall -c -o detectAndFixViruses.o detectAndFixViruses.c


#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean:
	@echo 'cleaning'
	rm -f *.o detectAndFixViruses
