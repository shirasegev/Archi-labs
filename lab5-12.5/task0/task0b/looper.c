#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void sigHandler(int sigNum) {
	printf("Received siganl: %s(%d)\n", strsignal(sigNum), sigNum);
	if (sigNum == SIGCONT) {
		signal(SIGTSTP, sigHandler);
	}
	else if (sigNum == SIGTSTP) {
		signal(SIGCONT, sigHandler);
	}
	signal(sigNum, SIG_DFL);
	raise(sigNum);
}


int main(int argc, char **argv){ 

	signal(SIGTSTP, sigHandler);
	signal(SIGINT, sigHandler);
	signal(SIGCONT, sigHandler);

	printf("Starting the program\n");

	while(1) {
		sleep(2);
	}

	return 0;
}