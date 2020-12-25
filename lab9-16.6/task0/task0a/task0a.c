#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "elf.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>

/* Global variables: */
int debug = 0;
void *addr; /* will point to the start of the memory mapped file */
struct stat fd_stat;
int Currentfd = -1;
Elf32_Ehdr *ehdr; /* this will point to the header structure */
int num_of_section_headers;

int main(int argc, char **argv){
    char *file_name;
    file_name = argv[1];
    if (Currentfd != -1){
        close(Currentfd);
        Currentfd = -1;
    }
    int fd;
    if ((fd = open(file_name, O_RDWR)) < 0){
        perror("error in open");
        exit(-1);
    }

    if (fstat(fd, &fd_stat) != 0){
        perror("stat failed");
        close(fd);
        exit(-1);
    }

    addr = mmap(NULL, fd_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED){
        perror("mmap failed");
        close(fd);
        exit(-4);
    }
    ehdr = (Elf32_Ehdr *)addr;
    num_of_section_headers = ehdr->e_shnum;

    if (strncmp("ELF", (char *)ehdr->e_ident + 1, 3) != 0 && ehdr->e_ident[0] == 0x7f){
        perror("Not an ELF file");
        close(fd);
        munmap(addr, fd_stat.st_size);
    }
    Currentfd = fd;

    Elf32_Phdr *progHraders = addr + ehdr->e_phoff;

    printf("Program Headers:\nType\tOffset\t\tVirtAddr\tPhysAddr\tFileSiz\t\tMemSiz\t\tFlg\tAlign\n");

    int i;
    for (i = 0; i < ehdr->e_phnum; i++){
        printf("%x\t%08x\t%08x\t%08x\t%08x\t%08x\t%x\t%04x\n",
        progHraders[i].p_type, progHraders[i].p_offset, progHraders[i].p_vaddr,
        progHraders[i].p_paddr, progHraders[i].p_filesz, progHraders[i].p_memsz,
        progHraders[i].p_flags, progHraders[i].p_align);
    }
    return 0;
}