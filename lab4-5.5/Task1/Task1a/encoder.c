#include "util.h"

#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_READ 3
#define SYS_WRITE 4

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define O_RDONLY 0
#define O_WRONLY 1
#define O_CREAT 64

char *input_name = "stdin";
char *output_name = "stdout";
char *r;

void debug_mode_prints(int result, char* SYS_NAME){
  r = itoa(result);
  system_call(SYS_WRITE, STDERR, "\n", 1);
  system_call(SYS_WRITE, STDERR, SYS_NAME, strlen(SYS_NAME));
  system_call(SYS_WRITE, STDERR, "\n", 1);
  system_call(SYS_WRITE, STDERR, r, strlen(r));
  system_call(SYS_WRITE, STDERR, "\n", 1);
  system_call(SYS_WRITE, STDERR, input_name, strlen(input_name));
  system_call(SYS_WRITE, STDERR, "\n", 1);
  system_call(SYS_WRITE, STDERR, output_name, strlen(output_name));
  system_call(SYS_WRITE, STDERR, "\n", 1);
}

int main(int argc, char **argv) {

  char c = 0, d = 0;
  int debug = 0;
  int encrypt = 0;
  char *key = 0;
  int index = 0;
  int keyLen = 0;

  int input = STDIN;
  int output = STDOUT;

  int result;
  int i;

  for (i = 1; i < argc; i++){
    if(strcmp(argv[i],"-D") == 0){
      debug = 1;
    }
    else if (strncmp(argv[i], "-i", 2) == 0){
      input = system_call(SYS_OPEN, &argv[i][2], O_RDONLY, 0777);
      input_name = &argv[i][2];
    }
    else if (strncmp(argv[i], "-o", 2) == 0){
      output = system_call(SYS_OPEN, &argv[i][2], O_CREAT | O_WRONLY, 0777);
      output_name = &argv[i][2];
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
      system_call(SYS_WRITE, STDOUT, "invalid parameter - ", 20);
      result = system_call(SYS_WRITE, STDOUT, argv[i], strlen(argv[i]));
      system_call(SYS_WRITE, STDOUT, "\n", 1);
      if (debug) debug_mode_prints(result, "SYS_WRITE");
	    return 1;
    }
  }

  if (debug) {
    for (i = 1; i < argc; i++){
      result = system_call(SYS_WRITE, STDERR, argv[i], strlen(argv[i]));
      system_call(SYS_WRITE, STDERR, "\n", 1);
      debug_mode_prints(result, "SYS_WRITE");
    }
  }
  
  while((result = system_call(SYS_READ, input, &c, 1)) > 0){

    if (debug) debug_mode_prints(result, "SYS_READ");

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
    result = system_call(SYS_WRITE, output, &d, 1);

    if (debug) debug_mode_prints(result, "SYS_WRITE");
  }

  if (input != STDIN){
    result = system_call(SYS_CLOSE, input);

    if (debug) debug_mode_prints(result, "SYS_CLOSE");

    if (result < 0) return 0x55;
  }
  if (output != STDOUT){
    result = system_call(SYS_CLOSE, output);

    if (debug) debug_mode_prints(result, "SYS_CLOSE");

    if (result < 0) return 0x55;
  }
  return 0;
}