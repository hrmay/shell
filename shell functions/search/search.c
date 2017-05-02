#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

int search(char dir[1024]) {
		DIR *direc;
		direc = opendir(dir);
		if (direc == NULL)
				return 1;
		//printf("checkpoint1\n");
		//printf("%s\n", dir);
		struct dirent *cdir;
		cdir = readdir(direc);
		while (cdir != NULL) {
			//printf("checkpoint2\n");
			struct stat tempStat;
			char temp[1024];
			strcpy(temp, dir);
			strcat(temp, "/");
			strcat(temp, cdir->d_name);
			stat(temp, &tempStat);
			if (S_ISDIR(tempStat.st_mode) != 0 && strcmp(cdir->d_name, ".") && strcmp(cdir->d_name, "..")) {
				search(temp);
			}
			if (strcmp(cdir->d_name, ".") && strcmp(cdir->d_name, ".."))
				printf("%s\t", cdir->d_name);
			cdir = readdir(direc);
		}
		
}

int main(int argc, char **argv) {
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));

	if (argc == 2) {
		strcat(cwd, "/");
		strcat(cwd, argv[1]);
	} else if (argc > 2)
			return 1;

	//printf("%s\n", cwd);
	int x = 0;
	x = search(cwd);
	if (x == 1) {
		printf("ERR: Specified directory does not exist.");
	}
	printf("\n");
}
