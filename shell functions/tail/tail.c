#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char **argv) {
	int lines = (int)argv[1];

	char printLines[lines][512];

	char cwd[1024];
	getcwd(cwd, sizeof(cwd));

	strcat(cwd, "/");
	strcat(cwd, argv[2]);

	printf("%s\n", cwd);

	int offset;
	
	int file = open(argv[2], O_RDONLY);

	offset = lseek(file, -10, SEEK_END);

	char buffer[1024];

	read(file, buffer, 1024);

	printf("%s\n", buffer);

}
