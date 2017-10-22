#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "arg_parse.h"

/* PROTOTYPES */

/* countArg
 * Helper function for arg_parse
 * takes in line and looks for whitespaces to determine how many arguments there are
 */
static int countArg(char* line);

char** arg_parse(char* line, int *argcp) {
  char* currArg;
	int argsInserted = 0;

	*argcp = countArg(line);
	char** arguments = (char**) malloc(*argcp*sizeof(char*));
	arguments[*argcp-1] = 0;

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
	aCounter++;
	return aCounter;
}

struct tempTarget* target_parse(char *line){
	tempTarget* curr = malloc(sizeof(tempTarget));
  char* currLine = strdup(line);
	char* colonP = strchr(currLine, ':');

	*colonP = '\0';
	colonP++;
	curr->targetName = strdup(currLine);
	curr->dependencies = strdup(colonP);
	return curr;
}
