#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <string.h>
#include <grp.h>
#include <dirent.h>

void modetostr(mode_t mode, char modestr[]){
	strcpy(modestr, "----------");
	if (S_ISDIR(mode)) modestr[0] = 'd';
	if (mode & 0400) modestr[1] = 'r';
	if (mode & 0200) modestr[2] = 'w';
	if (mode & 0100) modestr[3] = 'x';
	if (mode & 0040) modestr[4] = 'r';
	if (mode & 0020) modestr[5] = 'w';
	if (mode & 0010) modestr[6] = 'x';
	if (mode & 0004) modestr[7] = 'r';
	if (mode & 0002) modestr[8] = 'w';
	if (mode & 0001) modestr[9] = 'x';
}

int main(int ac, char *av[]){
	DIR *dp;
	struct dirent *p;
	struct stat buf;
	char modestr[11] = {0};

	if (ac == 1){
		dp = opendir(".");
	} else {
		if (chdir(av[1]) == -1){
			perror("cannot change directory");
			exit(1);
		}
		dp = opendir(".");
		if (dp == NULL){
			perror("cannot change directory");
			exit(1);
		}
	}
	
	// iterate directory content
	while((p = readdir(dp)) != NULL){
		if (p->d_name[0] != '.'){
			if(stat(p->d_name, &buf) == -1){
				perror("stat error");
				continue;
			}
		
			modetostr(buf.st_mode, modestr);
			printf("%s %d %s %s %6d %.12s %s\n",
				modestr, buf.st_nlink,
				getpwuid(buf.st_uid)->pw_name,
				getgrgid(buf.st_gid)->gr_name,
				buf.st_size,
				4 + ctime(&buf.st_mtime),
				p->d_name);
		}
	}
	
	closedir(dp);
}
