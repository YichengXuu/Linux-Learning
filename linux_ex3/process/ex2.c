#include <stdio.h>
#include <unistd.h>

int main(){
	int p1, p2;
	int i;	

	while((p1 = fork()) == -1);
	if(p1 == 0){
		for(i=0 ; i < 100; i++) {
			printf("Child 1 (b): iteration %d\n", i);
		}
	} else {
		while((p2 = fork()) == -1);
		if(p2 == 0) {
			for(i = 0; i < 100; i++){	
				printf("Child 2 (c): iteration %d\n", i);
			}
		} else {
			for(i = 0; i < 100; i++){	
				printf("Parent (a): iteration %d\n", i);
			}
		}
	}

	return 0;
}
