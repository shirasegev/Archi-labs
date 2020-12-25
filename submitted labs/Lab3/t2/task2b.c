#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    char* sig;
} virus;

typedef struct link link;
struct link {
    link *nextVirus;
    virus *vir;
};

char *fileName;
link *virus_list = NULL;

// return a virus that represents the next virus in the file.
virus* readVirus(FILE* file){
    virus *newVirus = (virus*)malloc(sizeof(virus));
    int n = fread(newVirus, 18, 1, file);
    if (n == 0){
        free(newVirus);
        return 0;
    }
    newVirus->sig = (char*)malloc(newVirus->SigSize);
    fread(newVirus->sig, 1, newVirus->SigSize, file);
    return newVirus;
}

/* prints the virus to the given output.
It prints the virus name (in ASCII),
the virus signature length (in decimal),
and the virus signature (in hexadecimal representation). */
void printVirus(virus* virus, FILE* output){
    fprintf(output, "Virus name: %s\nVirus size: %d\nsignature:\n", virus->virusName, virus->SigSize);
    int i;
    for (i = 0; i < virus->SigSize; i++){
        fprintf(output, "%02hhx ", (unsigned int)((virus->sig)[i]));
        if (i % 32 == 31){
            fprintf(output, "\n");
        }
    }
    if (i % 32 != 0){
        fprintf(output, "\n");
    }
    fprintf(output, "\n");
}

/* Print the data of every link in list to the given stream.
Each item followed by a newline character. */
void list_print(link *virus_list, FILE* file){
    while (virus_list != NULL){
        printVirus(virus_list->vir, file);
        virus_list = virus_list->nextVirus;
    }
}
     
/* Add a new link with the given data to the list 
    (either at the end or the beginning, depending on what your TA tells you),
    and return a pointer to the list (i.e., the first link in the list).
    If the list is null - create a new entry and return a pointer to the entry. */
link* list_append(link* virus_list, virus* data){
    link *newLink = (link*)malloc(sizeof(link));
    newLink->vir = data;
    newLink->nextVirus = NULL;

    link *head = virus_list;
    if (head == 0) {
        return newLink;
    }
    link *link = head;
    while (link->nextVirus != 0) {
        link = link->nextVirus;
    }
    link->nextVirus = newLink;
    return head;

}
     
/* Free the memory allocated by the list. */
void list_free(link *virus_list){
    while (virus_list != NULL){
        link *curr = virus_list;
        virus_list = virus_list->nextVirus;
        free(curr->vir->sig);
        free(curr->vir);
        free(curr);
    }
}

/* requests a signature file name parameter from the user after the user runs it by entering "1" */
void load_signatures() {
    printf("Pleas enter a signature file name:\n");
    char tmp[256];
    fgets(tmp, 256, stdin);
    char fileName[256];
    sscanf(tmp, "%s", fileName);
    FILE *signatures = fopen(fileName, "r");

    virus *virus = readVirus(signatures);
    while (virus != 0) {
        virus_list = list_append(virus_list, virus);
        virus = readVirus(signatures);
    }
    fclose(signatures);
}

/* print the signatures to the screen. If no file was loaded nothing is printed */
void print_signatures(){
    list_print(virus_list, stdout);
}

void detect_virus(char *buffer, unsigned int size, link *virus_list){
    link *link = virus_list;
    while (link != 0) {
        for (int i = 0; i <= size - link->vir->SigSize; i++){
            int result = memcmp(link->vir->sig, &buffer[i], link->vir->SigSize);
            if (result == 0){
                printf("%d %s %d\n", i, link->vir->virusName, link->vir->SigSize);
            }
        }
        link = link->nextVirus;
    }
}

void detect_viruses(){
    FILE *file = fopen(fileName, "r");
    char buffer[10240];
    unsigned int size = fread(buffer, 1, 10240, file);
    detect_virus(buffer, size, virus_list);
}

void kill_virus(char *fileName, int signitureOffset, int signitureSize){
    FILE *file = fopen(fileName, "r+");
    char buffer[signitureSize];
    for (int i = 0; i < signitureSize; i++){
        buffer[i] = 0x90;
    }
    fseek(file, signitureOffset, SEEK_SET);
    fwrite(buffer, 1, signitureSize, file);
    fclose(file);
}

void fix_file(){
    char buffer[16];
    int index, size;
    printf("Please enter the starting byte location in the suspected file:\n");
    fgets(buffer, 16, stdin);
    sscanf(buffer, "%d", &index);
    printf("Please enter the size of the virus signature:\n");
    fgets(buffer, 16, stdin);
    sscanf(buffer, "%d", &size);
    kill_virus(fileName, index, size);
}

void quit(){
    list_free(virus_list);
    exit(0);
}

struct fun_desc {
    char *name;
    void (*fun)();
};

int main(int argc, char **argv) {
    fileName = argv[1];

    struct fun_desc menu[] = { 
        {"Load signatures", load_signatures}, {"Print signatures", print_signatures},
            {"Detect viruses", detect_viruses}, {"Fix file", fix_file}, {"Quit", quit},
                {NULL, NULL} };

    int length = 0;
    while (menu[length].name != NULL){
        length++;
    }

    while (1) {
        for (int j = 0; j < length; j++){
            printf("%d) %s\n", j + 1, menu[j].name);
        }
        int choice;
        char tmp[10];
        fgets(tmp, 10, stdin);
        sscanf(tmp, "%d", &choice);
        menu[choice - 1].fun();
    }
    return 0;
}