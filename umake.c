/* CSCI 347 micro-make
 * 
 * 09 AUG 2017, Aran Clauson
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
/* CONSTANTS */
/* PROTOTYPES */
/* Parse Arguments
 * returns an array of pointers that point to characters in line
 */
char** arg_parse(char* line);
/* Process Line
 * line   The command line to execute.
 * This function interprets line as a command line.  It creates a new child
 * process to execute the line and waits for that process to complete. 
 */
void processline(char* line);
/* Main entry point.
 * argc    A count of command-line arguments 
 * argv    The command-line argument valus
 *
 * Micro-make (umake) reads from the uMakefile in the current working
 * directory.  The file is read one line at a time.  Lines with a leading tab
 * character ('\t') are interpreted as a command and passed to processline minus
 * the leading tab.
 */
int main(int argc, const char* argv[]) {
  FILE* makefile = fopen("./uMakefile", "r");
  size_t  bufsize = 0;
  char*   line    = NULL;
  ssize_t linelen = getline(&line, &bufsize, makefile);
 
  while(-1 != linelen) {
    if(line[linelen-1]=='\n') {
      linelen -= 1;
      line[linelen] = '\0';
    }
    if(line[0] == '\t') 
      processline(&line[1]);
    linelen = getline(&line, &bufsize, makefile);
  }
  free(line);
  return EXIT_SUCCESS;
}

/* Parse Argument
 * Should return an array of pointers
 */
char** arg_parse(char* line) {
	//Storing starting position	
	int aCounter = 0;
        int argFound = 0;
	char* start;
	start = line;
	//Iterate through line and find number of arguments
	while(*line != '\0'){
            //have 2 flags, 
            //one that detects when a char is found, iterate until you find a whitespace		
            if(*line != ' ' && argFound == 0){
                //char found
                argFound++;
                if(argFound == 1){
                    aCounter++; //arg found
                }
                printf("Arg found, pointer: %s \n", line);
                line++;
            }else{	
                //reset argfound
                argFound = 0;
                if(*line == '\0'){
                    //if pointer already at null dont increment
                    
                }else{
                    //skipping extra characters
                    printf("Skip, pointer: %s \n", line);
                    line++;
                }
            }
	}
        
	//reset pointer position 
	line = start;	
	//at this point you have your number of arguments

	//malloc your number of arguments +1
	aCounter++;
        printf("%d", aCounter);
        char** arguments = malloc(aCounter);
	char* currArg;
	int argsInserted;
	//Array is now created using malloc
	
	//Iterate through line again and store pointer location of first char of argument.
	//Store in char** array
	//Iterate until you find a whitespace or null pointer. Repeat
	while(*line != '\0'){
		//first character is not null, arg is not null
		if(*line == ' '){
			//Whitespace skip
			*line++;	
		}else{
			//Something other than a whitespace found
			//Store pointer in array
			currArg = line;
			arguments[argsInserted] = currArg;
			argsInserted++;
			
			//Once you have stored the pointer location of the first char, add in a null 				pointer in the first white space after that argument
			//Assume that the rest of the characters until the next whitespace are part 				of the argument. for now continue interating until next whitespace is found
			while(*line != ' '){
				line++;
			}
			//at this point should be at first new whitespace that is found
			if(*line == ' '){
				//first whitespace after arg found, replace with null pointer
				*line = '\0';				
			}
			line++;
		} 
	}
	//one extra null pointer at the end is required
	arguments[argsInserted] = '\0';
}
 
/* Process Line
 * 
 */
void processline (char* line) {
  
  //modify processline to use arg_parse
  printf("Calling arg_parse \n");
  char** test;
  test = arg_parse(line);

  const pid_t cpid = fork();
  switch(cpid) {
  case -1: {
    perror("fork");
    break;
  }
  case 0: {
    execlp(line, line, (char*)(0));
    perror("execlp");
    exit(EXIT_FAILURE);
    break;
  }
  default: {
    int   status;
    const pid_t pid = wait(&status);
    if(-1 == pid) {
      perror("wait");
    }
    else if (pid != cpid) {
      fprintf(stderr, "wait: expected process %d, but waited for process %d",
              cpid, pid);
    }
    break;
  }
  }
}
