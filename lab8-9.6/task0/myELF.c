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

void toggleDebug(){
    if (!debug){
        debug = 1;
        printf("Debug flag now on\n");
    }
    else{
        debug = 0;
        printf("Debug flag now off\n");
    }  
}

void examineELF(){
    if (Currentfd != -1){
        close(Currentfd);
        Currentfd = -1;
    }
    printf("Please enter the file name: ");
    char buff[100];
    fgets(buff, 100, stdin);
    buff[strlen(buff)-1] = 0;
    int fd;
    if ((fd = open(buff, O_RDWR)) < 0){
        perror("error in open");
        /*exit(-1);*/
        return;
    }

    if (fstat(fd, &fd_stat) != 0){
        perror("stat failed");
        close(fd);
        /*exit(-1);*/
        return;
    }

    addr = mmap(NULL, fd_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED){
        perror("mmap failed");
        close(fd);
        /*exit(-4);*/
        return;
    }
    ehdr = (Elf32_Ehdr *)addr;
    num_of_section_headers = ehdr->e_shnum;

    if (strncmp("ELF", (char *)ehdr->e_ident + 1, 3) != 0){
        perror("Not an ELF file");
        close(fd);
        munmap(addr, fd_stat.st_size);
        return;
    }
    Currentfd = fd;
    printf("1. Magic numbers: %c %c %c\n", ehdr->e_ident[EI_MAG1], ehdr->e_ident[EI_MAG2], ehdr->e_ident[EI_MAG3]);
    printf("2. Data encoding scheme: %d\n", ehdr->e_ident[EI_DATA]);
    printf("3. Entry point: %0#x\n",ehdr->e_entry);
    printf("4. File offset- section header: %d\n", ehdr->e_shoff);
    printf("5. Number of section header entries: %d\n", ehdr->e_shnum);
    printf("6. section header entry size: %d (bytes)\n", ehdr->e_shentsize); /* header size is const */
    printf("7. File offset- program header: %d\n", ehdr->e_phoff);
    printf("8. Number of program header entries.: %d\n", ehdr->e_phnum);
    printf("9. program header entry size:  %d (bytes)\n\n", ehdr->e_phentsize);
}

void quit(){
    if (debug){
        printf("quitting\n");
    }
    munmap(addr, fd_stat.st_size);
    if (Currentfd != -1){
        close(Currentfd);
    }
    exit(0);
}

struct fun_desc {
  char *name;
  void (*fun)();
};
 
int main(int argc, char **argv){
    struct fun_desc menu[] = { {"Toggle Debug Mode", toggleDebug}, 
                                {"Examine ELF File", examineELF},
                                {"Quit", quit}, {NULL, NULL} };

    while (1) {
        printf("Choose action:\n");
        int i;
        for (i = 0; menu[i].fun != NULL; i++){
            printf("%d-%s\n", i, menu[i].name);
        }
        int choice;
        char tmp[10];
        fgets(tmp, 10, stdin);
        sscanf(tmp, "%d", &choice);
        menu[choice].fun();
    }
    return 0;
}