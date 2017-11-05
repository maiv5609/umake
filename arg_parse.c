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

void env_process(char *line){
  char* eVariable;
  char* eValue;
  //dprintf(2, "eVariable before: %s\n", line);

  eVariable = line;

  while(*line != '\0'){
    if(*line == '='){
      *line = '\0';
      line++;
      eValue = line;
    }else{
      line++;
    }
  }
  //dprintf(2, "eVariable: %s\n", eVariable);
  //dprintf(2, "eValue: %s\n", eValue);
  setenv(eVariable, eValue, 1);
  if(getenv(eVariable)){
    //dprintf(2, "SET: %s\n\n", getenv(eVariable));
  }
}

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

  if(colonP == NULL){
    return NULL;
  }else{
    *colonP = '\0';
    colonP++;
    curr->targetName = strdup(currLine);
    curr->dependencies = strdup(colonP);
    return curr;
  }
}
