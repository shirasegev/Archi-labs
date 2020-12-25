#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int pipefd[2];
    pid_t c1pid;
    pid_t c2pid;
    int debug = 0;
    char* const lhs_arg_arr[] = {"ls", "-l", 0};
    char* const rhs_arg_arr[] = {"tail", "-n", "2", 0};

    for (int i = 1; i < argc; i++){
        if(strcmp(argv[i],"-d") == 0) debug = 1;
    }

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

    if (c1pid == 0) {    /* Child reads from pipe */
        close(STDOUT_FILENO);
        if (debug)
            fprintf(stderr, "(child1>redirecting stdout to the write end of the pipe…)\n");
        dup(pipefd[1]);
        close(pipefd[1]);          /* Close unused write end */
        
        if (debug)
            fprintf(stderr, "(child1>going to execute cmd: …)\n");
        execvp(lhs_arg_arr[0], lhs_arg_arr);
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
    if (c2pid == 0) {    /* Child 2 reads from pipe */
        close(STDIN_FILENO);
        dup(pipefd[0]);
        if (debug)
            fprintf(stderr, "(child2>redirecting stdin to the read end of the pipe…)\n");
        close(pipefd[0]);          /* Close unused read end */
        
        if (debug)
            fprintf(stderr, "(child2>going to execute cmd: …)\n");
        execvp(rhs_arg_arr[0], rhs_arg_arr);
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

        if (debug)
            fprintf(stderr, "(parent_process>exiting…)\n");
        exit(EXIT_SUCCESS);
    }
}