#include "dirent.h"
#include "util.h"

extern int system_call(int, ...);

#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_WRITE 4
#define SYS_GETDENTS 141

#define SYS_EXIT 1
#define ERROR_CODE 0x55

#define STDOUT 1
#define STDERR 2

#define O_RDONLY 0

typedef struct ent
{
  unsigned long inode;
  unsigned long offset;
  unsigned short len;
  char buf[1];
  char pad;
  char type;
} ent;

enum {
  DT_UNKNOWN = 0,
  #define DT_UNKNOWN  DT_UNKNOWN
  DT_FIFO = 1,
  #define DT_FIFO  DT_FIFO
  DT_CHR = 2,
  #define DT_CHR  DT_CHR
  DT_DIR = 4,
  #define DT_DIR  DT_DIR
  DT_BLK = 6,
  #define DT_BLK  DT_BLK
  DT_REG = 8,
  #define DT_REG  DT_REG
  DT_LNK = 10,
  #define DT_LNK  DT_LNK
  DT_SOCK = 12,
  #define DT_SOCK  DT_SOCK
  DT_WHT = 14,
  #define DT_WHT  DT_WHT
};

int call_system_call(int call_id, void* param1, void* param2, int param3, int debug) {
  int result = system_call(call_id, param1, param2, param3);
  if (debug) {
    char *r = itoa(call_id);
    system_call(SYS_WRITE, STDERR, "call id: ", 9);
    system_call(SYS_WRITE, STDERR, r, strlen(r));
    system_call(SYS_WRITE, STDERR, ", return code: ", 15);
    r = itoa(result);
    system_call(SYS_WRITE, STDERR, r, strlen(r));
    system_call(SYS_WRITE, STDERR, "\n", 1);
  }
  return result;
}

void print_type(struct ent *entp){
  char d_type = *((char*)entp + entp->len -1);
  char* type = (d_type == DT_REG) ?  "regular" :
              (d_type == DT_DIR) ?  "directory" :
              (d_type == DT_FIFO) ? "FIFO" :
              (d_type == DT_SOCK) ? "socket" :
              (d_type == DT_LNK) ?  "symlink" :
              (d_type == DT_BLK) ?  "block dev" :
              (d_type == DT_CHR) ?  "char dev" : "???";
  system_call(SYS_WRITE, STDOUT, "file type: ", 11);
  system_call(SYS_WRITE, STDOUT, type, strlen(type));
  system_call(SYS_WRITE, STDOUT, "\n", 1);
}

int main(int argc, char **argv) {
  char *r;
  int debug = 0;
  char *prefix = "";
  int type = 0;

  int i;
  for (i = 1; i < argc; i++){
    if(strcmp(argv[i],"-D") == 0){
      debug = 1;
    }
    else if (strncmp(argv[i], "-p", 2) == 0){
      prefix = argv[i]+2;
      type = 1;
    }
    else {
      system_call(SYS_WRITE, STDOUT, "invalid parameter - ", 20);
      system_call(SYS_WRITE, STDOUT, argv[i], strlen(argv[i]));
      system_call(SYS_WRITE, STDOUT, "\n", 1);
    }
  }

  if (debug) {
    for (i = 1; i < argc; i++){
      system_call(SYS_WRITE, STDERR, argv[i], strlen(argv[i]));
      system_call(SYS_WRITE, STDERR, "\n", 1);
    }
  }
 
  system_call(SYS_WRITE, STDOUT, "Flame 2 strikes!\n", 17);
  
  int file_directory = call_system_call(SYS_OPEN, ".", O_RDONLY, 0777, debug);
  if (file_directory < 0) system_call(SYS_EXIT, ERROR_CODE);

  char buff[8192];
  struct ent *entp = (struct ent *)buff;
  int count = call_system_call(SYS_GETDENTS, (void*)file_directory, (void*)entp, 8192, debug);

  int pos = 0;
  while (pos < count){
    struct ent *single_ent = (struct ent *)(buff+pos);

    if (debug){
      system_call(SYS_WRITE, STDERR, "dirent name: ", 13);
      system_call(SYS_WRITE, STDERR, single_ent->buf, strlen(single_ent->buf));
      system_call(SYS_WRITE, STDERR, "\n", 1);
      system_call(SYS_WRITE, STDERR, "dirent length: ", 15);
      r = itoa(single_ent->len);
      system_call(SYS_WRITE, STDERR, r, strlen(r));
      system_call(SYS_WRITE, STDERR, "\n", 1);
    }
    
    if (strncmp(single_ent->buf, prefix, strlen(prefix)) == 0) {
      system_call(SYS_WRITE, STDOUT, single_ent->buf, strlen(single_ent->buf));
      /* system_call(SYS_WRITE, STDOUT, " - ", 3);
      r = itoa(single_ent->inode);
      system_call(SYS_WRITE, STDOUT, r, strlen(r)); */
      system_call(SYS_WRITE, STDOUT, "\n", 1);
      if (type) print_type(single_ent);
    }
    pos = pos + single_ent->len;
  }

  int result = call_system_call(SYS_CLOSE, (void*)file_directory, (void*)0, 0, debug);
  if (result < 0) system_call(SYS_EXIT, ERROR_CODE);

  return 0;
}