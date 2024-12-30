#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

int main() {
	int fd[2];
	pid_t pid1, pid2;
	char OutPipe[100];
	char InPipe[100];

	if(pipe(fd) == -1) {
		perror("Pipe failed");
		exit(1);
	}

	while((pid1 = fork()) == -1);
	if(pid1 == 0){
		lockf(fd[1], 1, 0);
		sprintf(OutPipe, "Child 1 is sending message!");
		write(fd[1], OutPipe, 50);
		sleep(5);
		lockf(fd[1], 0, 0);
		exit(0);	
	} else {
		while((pid2 = fork()) == -1);
		if(pid2 == 0) {
			lockf(fd[1], 1, 0);
			sprintf(OutPipe, "Child 2 is sending message!");
			write(fd[1], OutPipe, 50);
			sleep(5);
			lockf(fd[1], 0, 0);
			exit(0);
		} else {
			wait(NULL);
			read(fd[0], InPipe, 50);
			printf("%s\n", InPipe);
			
			wait(NULL);
			read(fd[0], InPipe, 50);
			printf("%s\n", InPipe);
		
			exit(0);
		}
	}
}
