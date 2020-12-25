#include "util.h"

#define SYS_WRITE 4
#define STDOUT 1

void system_call(int, int, char *, int);

int main (int argc , char* argv[], char* envp[])
{
  system_call(SYS_WRITE, STDOUT, "hello world\n", 12);
  return 0;
}