#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

main(int ac, char *av[]){
	int fdin, fdout;
	char buf[1024];
	int n;

	if(ac!=3){
		printf("Usage: CMD filename\n");
		exit(1);
	}
	
	if((fdin=open(av[1],O_RDONLY))==-1){
		perror("cannot open");
		exit(1);
	}

	if((fdout=open(av[2],O_WRONLY|O_CREAT,0644))==-1){
		perror("cannot open");
	}

	while((n=read(fdin,buf,sizeof(buf)))>0){
		write(fdout, buf, n);
	}

	close(fdin);
	close(fdout);
}
