#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int debug;

typedef struct {
  char debug_mode;
  char display_mode;
  char file_name[128];
  int unit_size;
  unsigned char mem_buf[10000];
  size_t mem_count;
} state;

char* unit_to_format(int unit_size, char display_mode) {
    static char* formats[][4] = {{"%#hhd\n", "%#hd\n", "No such unit", "%#d\n"},
                                {"%#hhx\n", "%#hx\n", "No such unit", "%#x\n"}};
    return formats[(int)display_mode][unit_size-1];
} 

/* Prints the buffer to screen by converting it to text with printf */
void print_units(char* buffer, int count, state* s) {
    char* end = buffer + s->unit_size * count;
    printf(s->display_mode ? "Hexadecimal\n===========\n" : "Decimal\n=======\n");
    while (buffer < end) {
        int var = *((int*)(buffer));
        printf(unit_to_format(s->unit_size, s->display_mode), var);
        buffer += s->unit_size;
    }
} 

void memoryDisplay(state *s){
    printf("Please enter <units> <address>\n");
    int u;
    int addr;
    char tmp[256];
    fgets(tmp, 256, stdin);
    sscanf(tmp, "%d %x", &u, &addr);
    if (addr == 0){
        print_units((char *)s->mem_buf, u, s);
    }
    else{
        print_units((char *)addr, u, s);
    }  
}

void toggleDisplayMode(state* s){
    if (s->display_mode){
        s->display_mode = 0;
        printf("Display flag now off, decimal representation\n");
    }
    else {
        s->display_mode = 1;
        printf("Display flag now on, hexadecimal representation\n");
    }   
}

void loadIntoMemory(state* s) {   
    if (s->file_name == NULL){
        printf("ERROR: file does not exsist\n");
        return;
    }
    FILE *file = fopen(s->file_name, "r");
    if (file == NULL){
        printf("Failed to open file\n");
        return;
    }
    printf("Please enter <location> <length>\n");
    int location;
    int length;
    char tmp[256];
    fgets(tmp, 256, stdin);
    sscanf(tmp, "%X %d", &location, &length);

    if (s->debug_mode){
        printf("File name: %s\n", s->file_name);
        printf("Location: %x\n", location);
        printf("Length: %d\n", length);
    }
    fseek(file, location, SEEK_SET);
    fread(s->mem_buf, s->unit_size, length, file);
    printf("Loaded %d units into memory\n", length);
    fclose(file);
}

void toggleDebug(state* s) {
    if (!s->debug_mode){
        s->debug_mode = 1;
        printf("Debug flag now on\n");
    }
    else{
        s->debug_mode = 0;
        printf("Debug flag now off\n");
    }  
}

void setFileName(state* s) {
    printf("Please enter the file name: ");
    char buff[100];
    fgets(buff, 100, stdin);
    memcpy(s->file_name, buff, 100);
    s->file_name[strlen(buff)-1] = 0;
}

void setUnitSize(state* s) {
    printf("Please enter a number: ");
    int size;
    char tmp[10];
    fgets(tmp, 10, stdin);
    sscanf(tmp, "%d", &size);
    if(size == 1 || size == 2 || size == 4){
        s->unit_size = size;
        if (s->debug_mode){
            printf("Debug: set size to %d\n", size);
        }
    }
    else{
        printf("Entered value is not valid\n");
    }

}
/* Gets a char c,  and if the char is 'q' , ends the program with exit code 0. Otherwise returns c. */
void quit(state* s){
    if (s->debug_mode){
        printf("quitting\n");
    }
    exit(0);
}

struct fun_desc {
  char *name;
  void (*fun)(state*);
};
 
int main(int argc, char **argv){
    struct fun_desc menu[] = { {"Toggle Debug Mode", toggleDebug}, 
                                {"Set File Name", setFileName},
                                {"Set Unit Size", setUnitSize},
                                {"Load Into Memory", loadIntoMemory},
                                {"Toggle Display Mode", toggleDisplayMode},
                                {"Memory Display", memoryDisplay},
                                {"Quit", quit}, {NULL, NULL} };
    
    state new_state; // = (state*)malloc(sizeof(state));
    new_state.debug_mode = 0;
    new_state.unit_size = 1;
    new_state.mem_count = 0;
    new_state.display_mode = 0;

    while (1) {
        if (new_state.debug_mode){
            printf("Unit size: %d, File name: %s, Mem count: %d\n", new_state.unit_size, new_state.file_name, new_state.mem_count);
        }
        printf("Choose action:\n");
        for (int j = 0; menu[j].fun != NULL; j++){
            printf("%d-%s\n", j, menu[j].name);
        }
        int choice;
        char tmp[10];
        fgets(tmp, 10, stdin);
        sscanf(tmp, "%d", &choice);
        menu[choice].fun(&new_state);
    }

    return 0;
}