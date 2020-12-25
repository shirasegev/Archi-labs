#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include "LineParser.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

typedef struct pair{
    char* name;             /* the parsed command line*/
    char* value; 		    /* the process id that is running the command*/
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
        fclose((FILE*)pCmdLine->inputRedirect);
        fclose((FILE*)pCmdLine->outputRedirect);
        exit(-1);
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
                    perror("");
                    exit(EXIT_FAILURE);
                }
            } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
        }
    }
}

void mypipe(cmdLine *lhs_pipe, cmdLine *rhs_pipe){
    int pipefd[2];
    pid_t c1pid;
    pid_t c2pid;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if (debug)
        fprintf(stderr, "(parent_process>forking…)\n");
    c1pid = fork();
    if (debug)
        fprintf(stderr, "(parent_process>created process with id: %d)\n", c1pid);
    
    if (c1pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (c1pid == 0) {               /* Child reads from pipe */
        close(STDOUT_FILENO);
        if (debug)
            fprintf(stderr, "(child1>redirecting stdout to the write end of the pipe…)\n");
        dup(pipefd[1]);
        close(pipefd[1]);          /* Close unused write end */
        
        if (debug)
            fprintf(stderr, "(child1>going to execute cmd: …)\n");
        execvp(lhs_pipe->arguments[0], lhs_pipe->arguments);
        perror(NULL);
        _exit(EXIT_SUCCESS);

    }
    else {            /* Parent writes argv[1] to pipe */
        if (debug)
            fprintf(stderr, "(parent_process>closing the write end of the pipe…)\n");
        close(pipefd[1]);          /* Close the write end of the pipe */
    }

    if (debug)
        fprintf(stderr, "(parent_process>forking…)\n");
    c2pid = fork();
    if (debug)
        fprintf(stderr, "(parent_process>created process with id: %d)\n", c2pid);

    if (c2pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (c2pid == 0) {           /* Child 2 reads from pipe */
        close(STDIN_FILENO);
        dup(pipefd[0]);
        if (debug)
            fprintf(stderr, "(child2>redirecting stdin to the read end of the pipe…)\n");
        close(pipefd[0]);          /* Close unused read end */
        if (debug)
            fprintf(stderr, "(child2>going to execute cmd: …)\n");
        execvp(rhs_pipe->arguments[0], rhs_pipe->arguments);
        _exit(EXIT_SUCCESS);
    }
    else {            /* Parent writes argv[1] to pipe */
        if (debug)
            fprintf(stderr,"(parent_process>closing the read end of the pipe…)\n");
        close(pipefd[0]);          /* Close the read end of the pipe */
        if (debug)
            fprintf(stderr, "(parent_process>waiting for child processes to terminate…)\n");
        waitpid(c1pid, NULL, 0);                /* Wait for child */
        if (debug)
            fprintf(stderr, "(parent_process>waiting for child processes to terminate…)\n");
        waitpid(c2pid, NULL, 0);                /* Wait for child */
        // exit(EXIT_SUCCESS);
        return;
    }
}

int main(int argc, char **argv) {
    char *name;

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
            if (pCmdLine->next != NULL){
                replace_if_needed(pCmdLine);
                replace_if_needed(pCmdLine->next);
                mypipe(pCmdLine, pCmdLine->next);
            }
            else if (strcmp(pCmdLine->arguments[0], "quit") == 0){
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
                char value[256] = "\0";
                name = (char *)(pCmdLine->arguments[1]);
                for(int i=2; i<pCmdLine->argCount; i++){
                    strcat(value, pCmdLine->arguments[i]);
                    if(i!= pCmdLine->argCount - 1){
                        strcat(value, " ");
                    }
                }
                //value = (char *)(&(pCmdLine->arguments[1])+1);
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