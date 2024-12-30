#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int ac, char *av[]){
	int fd;
	char buf[1024];
	int n;
	const char *default_device = "/dev/pts/0";

	const char *device = (ac == 2) ? av[1] : default_device;

	fd = open(device, O_RDWR);
	if(fd == -1) {
		perror("cannot open");
		exit(1);
	}
	printf("Successfully opened %s\n", av[1]);

	const char *write_msg = "SkrSkr, this is a test write to the device file!\n";
	if(write(fd, write_msg, strlen(write_msg)) == -1){
		perror("Write to device file failed");
		close(fd);
		exit(1);
	}
	printf("Message written to the device: %s\n", write_msg);

	printf("Wating to read input from the device file (type something in the terminal)...\n");
	while((n = read(fd, buf, sizeof(buf)-1)) > 0 ) {
		buf[n] = '\0';
		printf("Read from device: %s\n", buf);
		break;
	}
	
	if(n == -1){
		perror("Read from device file failed");
	}		

	close(fd);
	return 0;
}
