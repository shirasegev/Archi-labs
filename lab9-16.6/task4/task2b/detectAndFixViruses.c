#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct link link;

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

struct link {
    link *nextVirus;
    virus *vir;
};

/*Structure that contains name of the function and pointer to that function.*/
struct fun_desc {
  char *name;
  void (*fun)();
};
//global variable
link *list_of_viruses = NULL;

int min(int x,int y){
  return (x<y) ? x : y;
}

void freeVirus(virus* virus){
  free(virus->sig);
  free(virus);
}

void freeLink(link * link){
  freeVirus(link->vir);
  free(link);
}

void printVirus(virus* virus, FILE* output){
  int sigsize = virus->SigSize;
  fprintf(output, "Virus name: %s\n", virus->virusName);
  fprintf(output, "Virus size: %d\n", sigsize);
  fprintf(output, "signature:\n");
  for(int i=0,j=1;i<sigsize;i++,j++){
    fprintf(output, "%02hhX ",virus->sig[i]);
    if(j%20 == 0 && j+1 < sigsize){
      fprintf(output, "\n");
    }
  }
  fprintf(output, "\n\n");
}

virus * readVirus(FILE *fptr){
  virus *v = (virus *)malloc(sizeof(virus));
  int readen = fread(v,sizeof(char), 18, fptr);
  if(readen < 18){
    free(v);
    return NULL;
  }
  v->sig = (unsigned char *) malloc(v->SigSize*sizeof(unsigned char*));
  fread(v->sig,sizeof(unsigned char),v->SigSize,fptr);
  return v;
}

void list_print(link *virus_list, FILE *output){
  link *currentLink = virus_list;
  printVirus(currentLink->vir,output);
  while(currentLink->nextVirus != NULL){
    currentLink = currentLink->nextVirus;
    printVirus(currentLink->vir,output);
  }
}

link * list_append(link* virus_list,virus *data){
  link *added = (link*) malloc(sizeof(link));
  link *currentLink = virus_list;
  added->vir = data;
  added->nextVirus = NULL; //going to be the last one in our list.
  if(virus_list == NULL){
    virus_list = added;
  }
  else{
    while(currentLink->nextVirus != NULL){
      currentLink = currentLink->nextVirus;
    }
    currentLink->nextVirus = added; //append instead of null at end.
  }
  return virus_list;//the start of the list is same.
}


void list_free(link *virus_list){
  if(virus_list == NULL){
    return;
  }
  link* currentLink = virus_list;
  link* nextLink = currentLink->nextVirus;
  freeLink(currentLink);
  while(nextLink != NULL){
    currentLink = nextLink;
    nextLink = currentLink->nextVirus;
    freeLink(currentLink);
  }
}

void detect_virus(char *buffer, unsigned int size, link *virus_list){
  link *currentLink = virus_list;
  while(currentLink != NULL){
    for(int i=0;i<= size - currentLink->vir->SigSize;i++){
      if(memcmp(&buffer[i],currentLink->vir->sig,currentLink->vir->SigSize) == 0){
        printf("Virus found!\n");
        printf("The starting byte location in the file is: %d\n", i);
        printf("The virus name: %s\n", currentLink->vir->virusName);
        printf("The size of the virus signature: %d\n", currentLink->vir->SigSize);
      }
    }
    currentLink = currentLink->nextVirus;
  }
}

void kill_virus(char *fileName, int signitureOffset, int signitureSize){
  FILE *fptr;
  if((fptr = fopen(fileName, "r+")) == NULL){
    printf("Could not open file: %s\n", fileName);
    return;
  }
  unsigned char fix[signitureSize];
  memset(fix,0x90,signitureSize);//0x90 is NOP
  fseek(fptr,signitureOffset,SEEK_SET);
  fwrite(fix,1,signitureSize,fptr);

  fclose(fptr);
}

void load_signatures(){
  char * filename = malloc(50*sizeof(char));
  FILE *fptr;
  int i=0;
  printf("Please enter filename:");
  fgets(filename,50,stdin);

  //enter null char instead of new line.
  while(filename[i] != '\n'){
    i++;
  }
  filename[i] = '\0';

  if((fptr = fopen(filename, "rb")) == NULL){
    printf("Could not open file: %s\n", filename);
    free(filename);
    return;
  }
  free(filename);
  if(list_of_viruses != NULL){
    list_free(list_of_viruses);
  }
  while(!feof(fptr)){
    virus * v = readVirus(fptr);
    if(v== NULL){
      break;
    }
    list_of_viruses = list_append(list_of_viruses,v);
  }
  fclose(fptr);
}

void print_signatures(){
  if(list_of_viruses == NULL){
    printf("Please load list first!\n");
    return;
  }
  list_print(list_of_viruses,stdout);
}

void detect_viruses(){
  char * filename = malloc(50*sizeof(char));
  FILE *fptr;
  int i=0;
  printf("Please enter filename:");
  fgets(filename,50,stdin);

  //enter null char instead of new line.
  while(filename[i] != '\n'){
    i++;
  }
  filename[i] = '\0';

  if((fptr = fopen(filename, "rb")) == NULL){
    printf("Could not open file: %s\n", filename);
    free(filename);
    return;
  }
  free(filename);
  char *buffer = (char *)malloc(1000*sizeof(char));
  int fileLength = fread(buffer,sizeof(char),1000,fptr);
  detect_virus(buffer,min(1000,fileLength),list_of_viruses);
  fclose(fptr);
  free(buffer);
}

void fix_file(){
  char * filename = malloc(50*sizeof(char));
  int i=0;
  int position;
  int virusSize;
  printf("Please enter filename:");
  fgets(filename,50,stdin);
  //enter null char instead of new line.
  while(filename[i] != '\n'){
    i++;
  }
  filename[i] = '\0';

  printf("Please enter the starting byte location of the virus:");
  scanf("%d",&position);
  printf("Please enter the size of the virus:");
  scanf("%d",&virusSize);
  fgetc(stdin);
  kill_virus(filename,position,virusSize);
  free(filename);
}

void quit(){
  list_free(list_of_viruses);
  exit(0);
}

int main(int argc, char **argv) {
  int option;
  struct fun_desc menu[] = { { "Load signatures", load_signatures }, { "Print signatures", print_signatures }, {"Detect viruses", detect_viruses},
  {"Fix file", fix_file}, { "Quit", quit }, { NULL, NULL } };
  int bound = sizeof(menu)/sizeof(struct fun_desc) - 1;

  while(1){
    printf("Please choose a function:\n");
    for(int i = 0; i < bound; i++){
      printf("%d) %s\n",i+1,menu[i].name);
    }
    printf("Option:");
    option = fgetc(stdin);
    //empty stdin from new line char
    fgetc(stdin);

    if(option >= '0' && option <= '9'){
      option -= '0';
    }
    //bound is the number of cells in menu, option in the range of 0 to bound-1
    if(option >= 1 && option <= bound){
      option -=1;//the menu is from 1 to 3 and the location is 0-2
      (menu[option].fun)();
      printf("\n");
    }
    else{
      printf("Not within bounds\n");
      printf("Exiting...\n");
      exit(1);
    }
  }
  return 0;
}
