#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include "LineParser.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#define TERMINATED -1
#define RUNNING 1
#define SUSPENDED 0

typedef struct process{
    cmdLine* cmd;               /* the parsed command line*/
    pid_t pid; 		            /* the process id that is running the command*/
    int status;                 /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	    /* next process in chain */
} process;

process *process_list = NULL;
int debug = 0;
char* status_arr[] = {"TERMINATED", "SUSPENDED", "RUNNING"};

void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
    process *new_process = (process*)malloc(sizeof(process));
    new_process->cmd = cmd;
    new_process->pid = pid;
    new_process->status = RUNNING;
    // insert the new process at the begining of process_list
    new_process->next = *process_list;
    *process_list = new_process;
}

void printProcessList(process** process_list){
    printf("PID\tCommand\tSTATUS\n");

    process *curr = *process_list;
    while (curr != NULL){
        printf("%d\t%s\t%s\n",
            curr->pid, curr->cmd->arguments[0], status_arr[curr->status + 1]);
        process *next = curr->next;

        curr = next;
    }
}

void execute(cmdLine *pCmdLine){
    pid_t w;
    int wstatus = 0;

    pid_t pid = fork();
    
    if (pid == 0) { /* Code executed by child */
        execvp(pCmdLine->arguments[0], pCmdLine->arguments);
        perror(NULL);
        exit(-1);
    }
    else if (pid == -1) { // fork failed
        perror("fork");
        exit(EXIT_FAILURE);  
    }
    else {  /* Code executed by parent */
        addProcess(&process_list, pCmdLine, pid);
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
                freeCmdLines(pCmdLine);
            }
            else if (strcmp(pCmdLine->arguments[0], "cd") == 0){
                if (chdir(pCmdLine->arguments[1]) < 0) 
                    perror(NULL);
                freeCmdLines(pCmdLine);
            }
            else if (strcmp(pCmdLine->arguments[0], "procs") == 0){
                printProcessList(&process_list);
                freeCmdLines(pCmdLine);
            }
            else {
                execute(pCmdLine);
            }
        }
    }
    return 0;
}