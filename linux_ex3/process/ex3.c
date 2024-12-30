#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	int count = 0;
	pid_t pid;
		
	while(1){
		pid = fork();
		if(pid < 0) {
			printf("Maximum number of processes: %d\n", count);
			exit(0);
		} 
		
		if(pid == 0) {
			exit(0);
		}
	
		count++;
			
	}

	return 0;
}
