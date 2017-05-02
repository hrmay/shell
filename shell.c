#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/****ERROR****
 * Prints out a standard error message and may kill the process
 * Parameters:
 *	int kill: Tells the function whether or not to kill the process
 *		1 - Kills the process
 *		Else (std:0) - Does not kill the process
 */
void error(int kill) {
	char error_message[30] = "An error has occured.\n";
	write(STDERR_FILENO, error_message, strlen(error_message));
	if (kill == 1)
		exit(1);
}

/*
int index(char s[512], char c) {
	char *temp;
	temp = strchr(s, c);
	int ind = (int)(temp - s);
	return ind;
}*/

/****CONTAINS****
 * Checks to see if the array contains a particular string
 * Parameters:
 *	char *s: The string to search for
 *	char a[][50]: The array to search
 *	int size: the size of the array
 * Return:
 *	Returns the index of the matching string if found; -1 if not
 */
int contains_s(char *s, char a[][50], int size) {
	int i;
	for (i=0; i<size; i++) {
		if (strcmp(s, a[i]) == 0)
			return i; //array contains the value
	}

	return -1; //array does not contain the value
}

/****CONTAINS****
 * Checks to see if the array contains a character anywhere within
 * Parameters:
 *	char c: The character to search for
 *	char a[][50]: The array to search
 *	int size: The size of the array
 * Return:
 *	Returns the index of the first string that contains the character if found; -1 if not
 */
int contains_c(char c, char a[][50], int size) {
	int i;
	for (i=0; i<size; i++) {
		if (strchr(a[i], c) != NULL)
			return i; //array contains the character somewhere
	}
	
	return -1; //array does not contain the character anywhere
}

/****EXECUTE****
 * Executes a function using execvp()
 * Parameters:
 *	char arg[20][50]: The list of arguments for the function, including the name and all optional arguments
 *	char filename[50]: The name of the file to output to
 *		stdout: The fucntion will print to the command line
 *		Else: The function will print to the file specified
 */
void execute(char arg[20][50], char filename[50]) {
	//BEGIN argument reduction
	//Removes blank or junk values from array of arguments	
	char c[50]; //command name
	strcpy(c, arg[0]); //copies command name from list of arguments (for readability)

	FILE *output;;
	if (strcmp(filename, "EMPTY") == 0)
			output = stdout;
	else{
			fclose(stdout);
			stdout = fopen(filename, "w");
	}

	//Gets amount of actual arguments, stopping when "END" is reached
	int j;
	for (j=0; j<20; j++) {
		if (strcmp(arg[j], "END") == 0)
			break;
	}
	char *args[j+1]; //Makes an array to hold all real arguments, plus a NULL terminator

	//Loops through both arrays and fills the new array (args) with the real values from arg
	int k;
	for (k=0; k<j; k++) {
		args[k] = (char*)malloc(sizeof(char*));
		strcpy(args[k], arg[k]);
	}
	args[j] = NULL; //Adds the NULL terminator to the end of args
	//END argument reduction

	//BEGIN execution
	//Executes the given function with the provided arguments
	if (strstr(c, ".py") != NULL && sizeof(args)/sizeof(char*) == 2) {
		char *argspy[3] = {"python", args[0], args[1]};
		int err = execvp("python", argspy);
		if (err < 0)
				error(0);
	} else {
		int err = execvp(c, args);
		if (err < 0)
				error(0);
	}
	//END execution
	exit(0); //Exits when done to prevent fork-bombing
}

/****MAIN****
 * The main function; takes in and parses commands and runs built-in commands
 * Parameters:
 *	int argc: Number of arguments (should only ever be 1 or 2)
 *	char *argv[]: Array of arguments (file to output to; stdin if blank)
 */
int main(int argc, char *argv[]) {
	int fromFile = 0; //1 if in batch mode
	FILE *file;
	//if there are any arguments, it's a batch file
	int counter = 0;
	if (argc > 1) {
		char test[512+1];
		fromFile = 1;
		//count lines in file
		file = fopen(argv[1], "r");
		while (fgets(test, 512+1, file))
			counter++;
		fclose(file); //close file after counting lines
		file = fopen(argv[1], "r"); //reopen file for actual parsing

	} else {
		//file = fopen("stdin", "r");
		file = stdin;
	}

	int internalCount = 0;
	//infinite loop to run shell
	for(;;) {
		
		//BEGIN reading command
		//Prints out shell header and takes in command from either the command line or a file
		char c[512+1]; //holds the command
		fprintf(stdout, "mysh> ");
		if (fromFile == 1) {
			if (internalCount++ == counter)
				exit(0);
		}
		fgets(c, 512+1, file);
		
		if (fromFile == 1) {
			fprintf(stdout, "%s", c);
		}
		//END reading command
		
		//BEGIN parsing
		//Breaks command into tokens, then loads tokens into an array
		char arguments[20][50]; //array of strings; holds the arguments; arguments[0] is the name of the command
		
		char *token;
		token = strtok(c, " \n\r\t");
		int counter = 0;
		int background = 0; //1 if background process
		int redirect = 0; //1 if redirection

		if (strchr(c, '>') != NULL) {
			//redirecting
			redirect = 1;
			
		}

		char outF[50];
		strcpy(outF, "EMPTY"); //will be replaced if we're redirecting
		
		//Loads tokens into array
		while (token != NULL) {
			//check if it's a background process
			if (strcmp(token, "&") == 0) {
				background = 1;
			}
			//check if it's a redirected process
			if (strcmp(token, ">") == 0) {
				redirect = 1;
				token = strtok(NULL, " \n\r\t");
				if (token == NULL)
						error(1);
				else {
						strcpy(outF, token);
						token = strtok(NULL, " \n\r\t");
						if (token != NULL)
								error(1);
						else
								break;
				}
			}
			else if (strchr(token, '>') != NULL) {
				//> connected to something
				redirect = 1;
				if (token[0] == '>') {
					token++;
					strcpy(outF, token);
					token = strtok(NULL, " \n\r\t");
					if (token != NULL)
							error(1);
					else
							break;
				}
			}
			//if it's neither, add this token to the arguments
			if (strcmp(token, "&") != 0 && strcmp(token, ">") != 0)
				strcpy(arguments[counter++], token);
			//then cycle tokens
			token = strtok(NULL, " \n\r\t");
		}
		strcpy(arguments[counter],"END");
	
		//int x = contains("ls", arguments, 20);
		//fprintf(stdout, "Contains ls: %d\n", x);
		
		//char outF[40];
		if (strchr(c, '>') != NULL) {
			redirect = 1;
			//char ccopy[512];
			//strcpy(ccopy, c);
			
			if (strstr(c, " > ") != NULL) {
				//">" is delimited by spaces
				//fprintf(stdout, "%d\n", index);
			} else if (strstr(c, " >") != NULL) {
				//">" is attached to file name
				int index = contains_c('>', arguments, 20);

			} else if (strstr(c, "> ") != NULL) {
				//">" is attached to command

			}
				
		}
		//END parsing

		//BEGIN built-in commands
		//Checks if the command is a built-in command and runs it if it is
		if (strcmp(arguments[0], "exit") == 0) {
			exit(0);
		} else if (strcmp(arguments[0], "cd") == 0) {
			if (strcmp(arguments[1], "END") == 0)
				chdir(getenv("HOME"));
			else if (strcmp(arguments[2], "END") != 0) {
				//incorrect number of arguments
				error(1);
			} else {
				char buffer[1024];
				getcwd(buffer, 1024);
				strcat(buffer, "/");
				strcat(buffer, arguments[1]);
				chdir(buffer);
			}
		} else if (strcmp(arguments[0], "pwd") == 0) {
			if (strcmp(arguments[1], "END") != 0){
				//incorrect number of arguments
				error(1);
			} else {
				char buffer[1024];
				getcwd(buffer, 1024);
				fprintf(stdout, "%s\n", buffer);
			}
		} else if (strcmp(arguments[0], "wait") == 0) {
			//check for number of arguments
			if (strcmp(arguments[1], "END") != 0) {
				//incorrect number of arguments
				error(1);
			} else {
				//WAIT to be implemented here
			}
		//END built-in commands
		
		//BEGIN execute functions
		//Executes an external function
		} else {
			//fork the process and execute a command
			int rc = fork();
			if (rc < 0)
				error(1); //fork failed
			else if (rc == 0) {	
				execute(arguments, outF); //runs the program
			} else {
				if (background == 0)
					wait();
				else { 
					background = 0;
				}
			}
		}
		//END execute functions
	}
	if (fromFile == 1)
			fclose(file);
	exit(0);
}
