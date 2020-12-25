#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  int c=0, d=0;
  int debug = 0;

  for(int i = 1; i < argc; i++){
    if(strcmp(argv[i],"-D") == 0){
      debug = 1;
    }
    else {
	    printf("invalid parameter - %s\n",argv[i]);
	    return 1;
    }
  }
  if (debug) {
    for (int i = 1; i < argc; i++){
      fprintf(stderr, "%s\n", argv[i]); 
    }
  }
  
  while((c=fgetc(stdin)) != EOF){
    if (c >= 'a' && c <= 'z'){
      d = c - 'a' + 'A';
    }
    else {
      d = c;
    }
    if (debug) {
      fprintf(stderr, "%d\t%d\n", c, d);
    }
    fputc(d, stdout);
  }
  return 0;
}