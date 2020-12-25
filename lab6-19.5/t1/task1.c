#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include "LineParser.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int debug = 0;

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
            }
            else if (strcmp(pCmdLine->arguments[0], "cd") == 0){
                if (chdir(pCmdLine->arguments[1]) < 0) 
                    perror(NULL);
            }
            else {
                execute(pCmdLine);
            }
        }
        freeCmdLines(pCmdLine);
    }
    return 0;
}