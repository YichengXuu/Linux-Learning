#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

void list_dir(const char *dir_name){
	DIR *dp;
	struct dirent *p;

	dp = opendir(dir_name);
	if(dp == NULL) {
		perror("cannot open directory");
		exit(1);
	}

	printf("\n%s\n", dir_name);

	while((p = readdir(dp)) != NULL) {
		// ignore hidden files and directory
		if (strcmp(p->d_name, ".") == 0 || strcmp(p->d_name, "..") == 0 ||
			p->d_name[0] == '.'){
			continue;
		}
		
		printf("%s ", p->d_name);
	}
	printf("\n");
	
	rewinddir(dp);
	while ((p = readdir(dp)) != NULL) {
		char path[1024];

		if (strcmp(p->d_name, ".") == 0 || strcmp(p->d_name, "..") == 0 ||
			p->d_name[0] == '.'){
			continue;
		}
		
		snprintf(path, sizeof(path), "%s/%s", dir_name, p->d_name);

		DIR *sub_dp = opendir(path);
		if(sub_dp != NULL){
			closedir(sub_dp);
			list_dir(path);
		}		
	}
	
	closedir(dp);	
}

int main(int ac, char *av[]){
	if(ac == 1){
		list_dir(".");
	} else {
		list_dir(av[1]);
	}

	return 0;
}	
