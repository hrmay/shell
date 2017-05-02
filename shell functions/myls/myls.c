#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char **argv){
		int foundl = 0;
		int directory = 0;
		if (argc > 3)
				return 1;
		else if (argc > 1) {
				if(strcmp(argv[1], "-l") == 0) {
						foundl = 1; //-l is the first argument
						if (argc == 3) 
								directory = 2; //there are two arguments; the directory must be the second
				} else if (argc > 2) { //two arguments
						if (strcmp(argv[2], "-l") == 0) {
								foundl = 2; //-l is the second argument
								directory = 1; //the directory is the first argument
						} else
								return 1; //two arguments, but they're not -l and directory
				}
				else
						directory = 1; //one argument; must be the dir
		}
		//if (argc == 1 || (argc == 2 && strcmp(argv[1], "-l") == 0)) {
			
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		
		if (directory != 0) {
				strcat(cwd, "/");
				strcat(cwd, argv[directory]);
		}

		//printf("%s\n\n", cwd);

		DIR *dir;
		dir = opendir(cwd);

		if (dir == NULL) {
				printf("ERR: Specified directory does not exist.\n");
				return 1;
		}

		struct dirent *cdir;
		cdir = readdir(dir);
		while (cdir != NULL) {
			if (foundl != 0) {
				struct stat tempStat;
				char temp[1024];
				strcpy(temp, cwd);
				strcat(temp, "/");
				strcat(temp, cdir->d_name);
				stat(temp, &tempStat);
				printf( (S_ISDIR(tempStat.st_mode)) ? "d" : "-");
				printf( (tempStat.st_mode & S_IRUSR) ? "r" : "-");
				printf( (tempStat.st_mode & S_IWUSR) ? "w" : "-");
				printf( (tempStat.st_mode & S_IXUSR) ? "x" : "-");
				printf( (tempStat.st_mode & S_IRGRP) ? "r" : "-");
				printf( (tempStat.st_mode & S_IWGRP) ? "w" : "-");
				printf( (tempStat.st_mode & S_IXGRP) ? "x" : "-");
				printf( (tempStat.st_mode & S_IROTH) ? "r" : "-");
				printf( (tempStat.st_mode & S_IWOTH) ? "w" : "-");
				printf( (tempStat.st_mode & S_IXOTH) ? "x" : "-");
				printf("\t");
				printf("%dj\t", tempStat.st_nlink);
				printf("%lu\t", (unsigned long int) tempStat.st_uid);
				printf("%lu\t", (unsigned long int) tempStat.st_gid);
				printf("%d\t", (int) tempStat.st_size);
				printf("%lld\t", (long long) tempStat.st_mtime);
				printf("%s\n", cdir->d_name);
			} else 
				printf("%s\t", cdir->d_name);

			cdir = readdir(dir);
		}
		printf("\n");
		
	/*	
	} else if (argc == 2 && strcmp(argv[1], "-l") == 0) {
		
	} else if (argc == 2 && strcmp(argv[1], "-l" != 0)) {
	
	} else if (argc == 3) {

	}*/
}
