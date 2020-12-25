#include "dirent.h"
#include <dirent.h>
#include "/usr/include/dirent.h"
#include "util.h"

#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_WRITE 4
#define SYS_GETDENTS 141

extern int system_call(int, ...);

#define SYS_EXIT 1
#define ERROR_CODE 0x55

#define STDOUT 1

#define O_RDONLY 0

typedef struct ent
{
  int inode;
  int offset;
  short len;
  char buf[1];
} ent;

int main(int argc, char **argv) {
  system_call(SYS_WRITE, STDOUT, "Flame 2 strikes!\n", 17);
  
  int file_directory = system_call(SYS_OPEN, ".", O_RDONLY, 0777);
  if (file_directory < 0) system_call(SYS_EXIT, ERROR_CODE);
  char buff[8192];
  struct ent *entp = (struct ent *)buff;
  int count = system_call(SYS_GETDENTS, file_directory, buff, 8192);

  int pos = 0;
  while (pos < count){
    struct ent *single_ent = (struct ent *)(buff+pos);
    system_call(SYS_WRITE, STDOUT, single_ent->buf, strlen(single_ent->buf));
    system_call(SYS_WRITE, STDOUT, "\n", 1);
    pos = pos + single_ent->len;
  }

  int result;

  result = system_call(SYS_CLOSE, file_directory);
  if (result < 0) system_call(SYS_EXIT, ERROR_CODE);

  return 0;
}