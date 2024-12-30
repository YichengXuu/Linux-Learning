#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

int pid1, pid2;
int EndFlag = 0;

void IntDelete() {
	kill(pid1, SIGUSR1);
	kill(pid2, SIGUSR2);
	EndFlag = 1;
}

void Int1(){
	printf("Child process 1 is killed by parent!\n");
	exit(0);
}

void Int2(){
	printf("Child process 2 is killed by parent!\n");
	exit(0);
}

int main(){
	int exitcode;

	// Parent ignore SIGQUIT
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);

	while((pid1 = fork()) == -1);
	if(pid1 == 0) {
		signal(SIGUSR1, Int1);
		signal(SIGINT, SIG_IGN);
		pause();
		exit(0);
	} else {
		while((pid2 = fork()) == -1);
		if(pid2 == 0){
			signal(SIGUSR2, Int2);
			signal(SIGINT, SIG_IGN);
			pause();
			exit(0);
		} else {
			signal(SIGINT, IntDelete);
			while(EndFlag == 0) {
				pause();
			}
			waitpid(-1, &exitcode, 0);
			waitpid(-1, &exitcode, 0);
			printf("Parent process is killed!\n");
			exit(0);
		}
	}

	return 0;
}

