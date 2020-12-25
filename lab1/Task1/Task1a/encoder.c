#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int c=0;
  
  while((c=fgetc(stdin)) != EOF){
    if (c >= 'a' && c <= 'z'){
      c = c - 'a' + 'A';
    }
    fputc(c, stdout);
  }
  return 0;
}