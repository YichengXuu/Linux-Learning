#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>

void copy_file(const char *src, const char *dest) {
	int fdin, fdout;
	char buf[1024];
	ssize_t n;

	if((fdin = open(src, O_RDONLY)) == -1){
		perror("cannot open source file");
		exit(1);
	}

	if((fdout = open(dest, O_WRONLY|O_CREAT|O_TRUNC, 0644)) == -1 ) {
		perror("cannot create destination file");
		exit(1);	
	}

	while((n = read(fdin, buf, sizeof(buf))) > 0 ) {
		if(write(fdout, buf, n) == -1) {
			perror("write error");
			close(fdin);
			close(fdout);
			exit(1);
		}
	}

	if(n == -1) {
		perror("read error");
	}

	close(fdin);
	close(fdout);
}


void copy_directory(const char *src, const char *dest){
	DIR *dp;
	struct dirent *entry;
	char src_path[1024];
	char dest_path[1024];
	
	if ((dp = opendir(src)) == NULL ) {
		perror("cannot open source directory");
		exit(1);
	}

	if(mkdir(dest, 0755) == -1 && errno != EEXIST ) {
		perror("cannot create destination directory");
		exit(1);
	}

	while((entry = readdir(dp)) != NULL) {
		if(strcmp(entry->d_name, ".") == 0 ||
			strcmp(entry->d_name, "..") == 0){
			continue;
		}

		snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
		snprintf(dest_path, sizeof(dest_path), "%s/%s", dest, entry->d_name);
	
		struct stat statbuf;
		if( stat(src_path, &statbuf) == -1) {
			perror("stat error");
			continue;
		}

		if(S_ISDIR(statbuf.st_mode)) {
			copy_directory(src_path, dest_path);
		} else if (S_ISREG(statbuf.st_mode)) {
			copy_file(src_path, dest_path);
		}
	}

	closedir(dp);
}

int main(int ac, char *av[]) {
	struct stat statbuf;

	if(ac != 3) {
		printf("UsageL %s <source> <destination>\n", av[0]);
		exit(1);
	}

	if(stat(av[1], &statbuf) == -1) {
		perror("stat error");
		exit(1);
	}

	if(S_ISDIR(statbuf.st_mode)) {
		copy_directory(av[1], av[2]);
	} else if (S_ISREG(statbuf.st_mode)) {
		copy_file(av[1], av[2]);
	} else {
		printf("Unsupported file type.\n");
		exit(1);
	}

	return 0;
}	

