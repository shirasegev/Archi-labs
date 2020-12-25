#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// helper function. Prints length bytes from memory location buffer, in hexadecimal format
void PrintHex(char *buffer, int length){
  for (int i = 0; i < length; i++){
    printf("%02x ", (unsigned char)buffer[i]);
  }
}

int main(int argc, char **argv) {
  FILE *input = fopen(argv[1], "r");
  
  int n;
  int buffsize = 32;
  char *buffer = (char*)malloc(buffsize);
  do {
    n = fread(buffer, 1, buffsize, input);
    PrintHex(buffer, n);
  } while (n == buffsize);
  printf("\n");
  
  fclose(input);
  return 0;
}