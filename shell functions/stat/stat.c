#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

int main (int argc, char **argv) {
		struct stat tempStat;
		
		if (argc > 2)
			return 1;
		else if (argc == 1) {
			char cwd[1024];
			getcwd(cwd, sizeof(cwd));
			stat(cwd, &tempStat);
			printf("Information for %s\n", cwd);
		} else if (argc == 2) {
			stat(argv[1], &tempStat);
			printf("Information for %s\n", argv[1]);
		}
		
		printf("----------------------------------------------------\n");
		printf("File Size: %jd bytes\n", tempStat.st_size);
		printf("Number of Links: %jd\n", tempStat.st_nlink);
		printf("File inode: %jd\n", tempStat.st_ino);
	
		printf("File Permissions:");
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
		
		printf("\n");

		return 0;
}
