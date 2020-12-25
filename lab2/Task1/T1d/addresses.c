#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char** argv){

    int iarray[] = {1,2,3};
    char carray[] = {'a','b','c'};
    int* iarrayPtr = iarray;
    char* carrayPtr = carray;

    void *p;

    printf("%d, %d, %d\n", *iarrayPtr, *(iarrayPtr+1), *(iarrayPtr+2));
    printf("%c, %c, %c\n", *carrayPtr, *(carrayPtr+1), *(carrayPtr+2));
    printf("%p\n", p);

    return 0;
}