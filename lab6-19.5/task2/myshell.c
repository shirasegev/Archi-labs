#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include "LineParser.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

typedef struct pair{
    char* name;               /* the parsed command line*/
    char* value; 		            /* the process id that is running the command*/
    struct pair *next;	    /* next process in chain */
} pair;

pair *pair_list = NULL;
int debug = 0;

int override_if_exists(char *name, char *value){
    int overroed = 0;
    pair *curr = pair_list;
    while (curr != NULL && !overroed){
        if (strcmp(curr->name, name) == 0){
            free(curr->value);
            curr->value = (char*)malloc(strlen(value)+1);
            strcpy(curr->value, value);
            overroed = 1;
        }
        curr = curr->next;
    }
    return overroed;
}

void addPair(char *name, char* value){
    int exists = override_if_exists(name, value);
    if (!exists){
        pair *new_pair = (pair*)malloc(sizeof(pair));
        new_pair->name = (char*)malloc(strlen(name)+1);
        strcpy(new_pair->name, name);
        new_pair->value = (char*)malloc(strlen(value)+1);
        strcpy(new_pair->value, value);
        new_pair->next = NULL;
        pair *head = pair_list;
        if (head == NULL){
            pair_list = new_pair;
            return;
        }
        pair *pair = head;
        while (pair->next != NULL){
            pair = pair->next;
        }
        pair->next = new_pair;
    }
}

void printVarsList(){
    printf("NAME\tVALUE\n");
    pair *curr = pair_list;
    while (curr != NULL){
        printf("%s\t%s\n", curr->name, curr->value);
        curr = curr->next;
    }
}

char* get_value_by_name(char *name){
    pair *curr = pair_list;
    while (curr != NULL){
        if (strcmp(curr->name, name) == 0){
            return curr->value;
        }
        curr = curr->next;
    }
    return NULL;
}

/* Free the memory allocated by the list. */
void list_free(){
    while (pair_list != NULL){
        pair *curr = pair_list;
        pair_list = pair_list->next;
        free(curr->name);
        free(curr->value);
        free(curr);
    }
}

void replace_if_needed(cmdLine* pCmdLine){
    for (int i = 0; i < pCmdLine->argCount; i++){
        if (strncmp(pCmdLine->arguments[i], "$", 1) == 0){
            char *replace = get_value_by_name((char*)(pCmdLine->arguments[i]+1));
            if (replace == NULL){
                fprintf(stderr, "ERROR: activating a variable that does not exist. You must set first: %s\n",
                    (char*)(pCmdLine->arguments[i]+1));
                // perror(NULL);
            }
            else {
                replaceCmdArg(pCmdLine, i, replace);
            }
        }
    }
}

void execute(cmdLine *pCmdLine){
    pid_t w = 0;
    int wstatus = 0;

    pid_t pid = fork();
    
    if (pid == 0) { /* Code executed by child */

        if (pCmdLine->inputRedirect != NULL){
            fclose(stdin);
            fopen(pCmdLine->inputRedirect, "r");
        }
        if (pCmdLine->outputRedirect != NULL){
            fclose(stdout);
            fopen(pCmdLine->outputRedirect, "w");
        }
        execvp(pCmdLine->arguments[0], pCmdLine->arguments);
        perror(NULL);
        if (pCmdLine->inputRedirect != NULL){
            fclose((FILE*)pCmdLine->inputRedirect);
        }
        if (pCmdLine->outputRedirect != NULL){
            fclose((FILE*)pCmdLine->outputRedirect);
        }
        exit(EXIT_SUCCESS);
    }
    else if (pid == -1) { // fork failed
        perror("fork");
        exit(EXIT_FAILURE);  
    }
    else {  /* Code executed by parent */
        if (debug)
            fprintf(stderr, "PID: %d - Executing command: %s\n", pid, pCmdLine->arguments[0]);
        if (pCmdLine->blocking) {
            do {
                w = waitpid(pid, &wstatus, WUNTRACED); // | WCONTINUED);
                if (w == -1) {
                    perror("waitpid");
                    exit(EXIT_FAILURE);
                }
            } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
        }
    }
}

int main(int argc, char **argv) {
    char *name;
    char *value;

    int quit = 0;
    char curr_dir[PATH_MAX];

    for (int i = 1; i < argc; i++){
        if(strcmp(argv[i],"-d") == 0) debug = 1;
    }
    
    char input[2048];
    while (!quit){
        getcwd(curr_dir, PATH_MAX);
    
        printf("%s> ", curr_dir);
        fgets(input, 2048, stdin);
        cmdLine* pCmdLine = parseCmdLines(input);
        
        if (pCmdLine != NULL) {
            if (strcmp(pCmdLine->arguments[0], "quit") == 0){
                quit = 1;
                list_free();
            }
            else if (strcmp(pCmdLine->arguments[0], "cd") == 0){
                if (strncmp(pCmdLine->arguments[1], "~", 1) == 0){
                    if (chdir(getenv("HOME")) < 0) 
                        perror(NULL);
                }
                else {
                    replace_if_needed(pCmdLine);
                    if (chdir(pCmdLine->arguments[1]) < 0) 
                        perror(NULL);
                }
            }
            else if (strcmp(pCmdLine->arguments[0], "set") == 0){
                name = (char *)(pCmdLine->arguments[1]);
                value = (char *)(pCmdLine->arguments[2]);
                addPair(name, value);
            }
            else if (strcmp(pCmdLine->arguments[0], "vars") == 0){
                printVarsList();
            }
            else {
                replace_if_needed(pCmdLine);
                execute(pCmdLine);
            }
        }
        freeCmdLines(pCmdLine);
    }
    return 0;
}