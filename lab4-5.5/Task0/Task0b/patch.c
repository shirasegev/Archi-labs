#include "util.h"

#define SYS_LSEEK 19
#define SYS_OPEN 5
#define SYS_CLOSE 6

#define SYS_WRITE 4
#define STDOUT 1

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define O_RDONLY 0
#define O_WDONLY 1
#define O_RDRW 2
#define O_APPEND
#define O_TRUNC
#define O_CREAT

int main (int argc , char* argv[], char* envp[])
{
  char *file_name = argv[1];
  char *x_name = argv[2];
  int result;
  int file_descriptor = system_call(SYS_OPEN, file_name, O_RDRW, 0777);
  if (file_descriptor < 0) return 0x55;
  result = system_call(SYS_LSEEK, file_descriptor, 0x291, SEEK_SET);
  if (result < 0) return 0x55;
  result = system_call(SYS_WRITE, file_descriptor, x_name, strlen(x_name));
  if (result < 0) return 0x55;
  result = system_call(SYS_CLOSE, file_descriptor);
  if (result < 0) system_call(1, 0x55);
  return 0;
}