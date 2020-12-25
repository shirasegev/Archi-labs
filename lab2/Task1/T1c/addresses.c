#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char** argv){

    int iarray[3];
    float farray[3];
    double darray[3];
    char carray[3];

    printf("%p %p\n",iarray, iarray+1);
    printf("%p %p\n",farray, farray+1);
    printf("%p %p\n",darray, darray+1);
    printf("%p %p\n",carray, carray+1);

    // printf("%p %p %p\n",&iarray[0], &iarray[1], &iarray[2]);
    // printf("%p %p %p\n",&farray[0], &farray[1], &farray[2]);
    // printf("%p %p %p\n",&darray[0], &darray[1], &darray[2]);
    // printf("%p %p %p\n",&carray[0], &carray[1], &carray[2]);

    return 0;
}