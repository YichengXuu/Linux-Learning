#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	char buf[1024];

	if (getcwd(buf, sizeof(buf)) == NULL) {
		perror("getcwd error");
		exit(1);
	}

	printf("%s\n", buf);

	return 0;
}
