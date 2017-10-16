#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "arg_parse.h"

/* PROTOTYPES */
/* Helper function
 * arg_parse will use this to count number of arguments
 */
static int countArg(char* line);

/* Helper function for arg_parse
 * Takes in copy of pointer and counts number of arguments
 */
static int countArg(char* line){
	int aCounter = 0;
				int argFound = 0;
	while(*line != '\0'){
						if(*line != ' ' && argFound == 0){
								argFound++;
								if(argFound == 1){
										aCounter++;
								}
								line++;
						}else{
								if(*line != '\0' && !isspace(*line)){
										argFound++;
										line++;
								}else if (isspace(*line)){
										argFound = 0;
										line++;
								}
						}
	}
				return aCounter;
}

/* Parse Argument
 * Takes original pointer and processes it to parse arguments
 *
 */
char** arg_parse(char* line) {
  char* currArg;
	int argsInserted = 0;
	int aCounter = 0;

	aCounter = countArg(line);
	aCounter++;
	char** arguments = (char**) malloc(aCounter*sizeof(char*));
	arguments[aCounter-1] = 0;

	while(*line != '\0'){
		if(isspace(*line)){
			line++;
		}else{
			currArg = line;
			arguments[argsInserted] = currArg;
			argsInserted++;
			while(!isspace(*line) && *line != '\0'){
					line++;
			}
			if(isspace(*line)){
					*line = '\0';
					line++;
			}
		}
	}
	return arguments;
}
