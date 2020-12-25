#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* create a child process that sends the message "hello" to its parent process.
The parent then prints the incoming message and terminates.*/

/* The array pipefd is used to return two file descriptors referring to the ends of the pipe.
    pipefd[0] refers to the read end of the pipe.
    pipefd[1] refers to the write end of the pipe. */

/* Data written to the write end of the pipe is buffered by the
    kernel until it is read from the read end of the pipe. */

int main(int argc, char *argv[]){
    int pipefd[2];
    pid_t cpid;
    char buf;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {    /* Child writes "hello" to pipe */ 
        close(pipefd[0]);                               /* Close unused read end */
        write(pipefd[1], "hello\n", 6);
        close(pipefd[1]);                               /* Reader will see EOF */
        _exit(EXIT_SUCCESS);

    }
    else {      /* Parent reads from pipe */
        close(pipefd[1]);                               /* Close unused write end */
        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);
        close(pipefd[0]);

        wait(NULL);                                     /* Wait for child */
        exit(EXIT_SUCCESS);
    }
}