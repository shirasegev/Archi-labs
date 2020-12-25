#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  int c=0, d=0;
  int debug = 0;
  int encrypt = 0;
  char *key = 0;
  int index = 0;
  int keyLen = 0;
  FILE *output = stdout;

  for(int i = 1; i < argc; i++){
    if(strcmp(argv[i],"-D") == 0){
      debug = 1;
    }
    else if (strncmp(argv[i], "-o", 2) == 0){
      output = fopen(&argv[i][2], "w");
    }
    else if (strncmp(argv[i], "-e", 2) == 0){
      encrypt = -1;
      key = &argv[i][2];
      keyLen = strlen(key);
    }
    else if (strncmp(argv[i], "+e", 2) == 0){
      encrypt = 1;
      key = &argv[i][2];
      keyLen = strlen(key);
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
    if (encrypt == 0) {
      if (c >= 'a' && c <= 'z'){
        d = c - 'a' + 'A';
      }
      else {
        d = c;
      }
    }
    else {
      if (c == '\n'){
        d = c;
        index = 0;
      }
      else {
        d = c + encrypt * (key[index] - '0');
        index = (index + 1) % keyLen;
      }
    }
    if (debug) {
      fprintf(stderr, "%d\t%d\n", c, d);
    }
    fputc(d, output);
  }

  if (output != stdout){
    fclose(output);
  }
  return 0;
}