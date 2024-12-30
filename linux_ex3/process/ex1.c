#include <stdio.h>
#include <unistd.h>

int main(){
	int p1, p2;
	
	while((p1 = fork()) == -1);
	if(p1 == 0){
		putchar('b');
		putchar('\n');
	} else {
		while((p2 = fork()) == -1);
		if(p2 == 0) {
			putchar('c');
			putchar('\n');
		} else {
			putchar('a');
			putchar('\n');
		}
	}

	return 0;
}
