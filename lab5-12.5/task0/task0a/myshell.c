#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include "LineParser.h"
#include <unistd.h>

void execute(cmdLine *pCmdLine){
    execvp(pCmdLine->arguments[0], &pCmdLine->arguments[0]);
    perror(NULL);
    exit(-1);
}

int main(int argc, char **argv) {
    int quit = 0;
    char curr_dir[PATH_MAX];

printf("Starting\n");
    char input[2048];
    while (!quit){
        getcwd(curr_dir, PATH_MAX);
    
        printf("%s> ", curr_dir);
        fgets(input, 2048, stdin);
        //char c;
        //scanf("%s%c", input, &c);
        cmdLine* pCmdLine = parseCmdLines(input);
        if (pCmdLine != NULL) {
            if (strcmp(pCmdLine->arguments[0], "quit") == 0){
                quit = 1;
            }
            else {
                execute(pCmdLine);
            }
            freeCmdLines(pCmdLine);
        }
    }

    return 0;
}