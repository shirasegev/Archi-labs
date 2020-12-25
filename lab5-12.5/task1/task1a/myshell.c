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
        execvp(pCmdLine->arguments[0], &pCmdLine->arguments[0]);
        perror(NULL);
        exit(-1);
    }
    else if (pid == -1) { // fork failed
        perror("fork");
        exit(EXIT_FAILURE);  
    }
    else {  /* Code executed by parent */
        if (debug)
            fprintf(stderr, "PID: %d - Executing command: %s\n", pid, pCmdLine->arguments[0]);
        do {
            w = waitpid(pid, &wstatus, WUNTRACED); // | WCONTINUED);
            if (w == -1) {
                perror("");
                exit(EXIT_FAILURE);
            }
            /*
            if (WIFEXITED(wstatus)) {
                printf("exited, status=%d\n", WEXITSTATUS(wstatus));
            } else if (WIFSIGNALED(wstatus)) {
                printf("killed by signal %d\n", WTERMSIG(wstatus));
            } else if (WIFSTOPPED(wstatus)) {
                printf("stopped by signal %d\n", WSTOPSIG(wstatus));
            } else if (WIFCONTINUED(wstatus)) {
                printf("continued\n");
            }
            */
        } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
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
                freeCmdLines(pCmdLine);
            }
            else {
                execute(pCmdLine);
            }
        }
    }
    return 0;
}