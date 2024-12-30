#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

int wait_mark;

void waiting(){
	while(wait_mark != 0);
}

void stop(){
	wait_mark = 0;
}

int main(){
	pid_t p1, p2;


	while((p1 = fork()) == -1);
	if(p1 == 0) {
		wait_mark = 1;
		signal(10, stop);
		waiting();
		lockf(1, 1, 0);
		printf("Child process 1 is killed by parent!\n");
		lockf(1, 0, 0);
		exit(0);
	} else {
		while((p2 = fork()) == -1);
		if(p2 == 0){
			wait_mark = 1;
			signal(12, stop);
			waiting();
			lockf(1, 1, 0);
			printf("Child process 2 is killed by parent!\n");
			lockf(1, 0, 0);
			exit(0);
		} else {
			signal(SIGINT, stop);
			printf("Press CTRL+C to terminate the processes...\n");
			wait_mark = 1;
			waiting();
			kill(p1, 10);
			kill(p2, 12);

			wait(NULL);
			wait(NULL);
			printf("Parent process is killed!\n");
			exit(0);
		}
	}

	return 0;
}










