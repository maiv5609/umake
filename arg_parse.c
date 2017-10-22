#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "arg_parse.h"

/* CURRENTLY WORKING ON: ISSUE 5
 * 1 umake.o: umake.c arg_parse.h
 * 2			gcc -c umake.c
 *
 * umake.o is the target while umake.c and arg_parse.h are the dependencies
 * we are not doing anything with the dependencies yet but need to store them
 * line 2 is a rule that the target will need to execute and can have multiple rules
 *
 * in target.c and target.h you need to collect and store the:
 *			target
 *			dependencies
 *			rules
 *
 * "This should include some sort of sequential container of targets"
 * Initally I should probably make a linked list of targets.
 *
 * Each target could be a struct that contains:
 *			an array containing the dependencies
 *			a char* array with each entry containing a single rule (line)
 *
 * After you collect the target information umake will read from it's
 * command-line argments(file?)
 * when it does this you will need to compare the current command-line argument
 * being read and use strcmp(3) to compare that to my first target and if it
 * doesn't match the first target move onto the next one.
 * execute the target's rules if strcmp(3) returns 0 (the two strings are equal).
 *
 * Completed issues (I think):
 * 				Issue 1
 *				Issue 2
 *				Issue 3
 *				Issue 4
 */


/* PROTOTYPES */

/* Helper function
 * arg_parse will use this to count number of arguments
 */
static int countArg(char* line);

/* Parse Argument
 * Takes original pointer and processes it to parse arguments
 * by checking for whitespaces and null terminators inbetween characters
 */
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
	aCounter++;
	return aCounter;
}

/* Parsing Function
 * arg_parse will use this to parse a line if it detects that it is a target
 */
struct tempTarget* target_parse(char *line){
	tempTarget* curr = malloc(sizeof(tempTarget));
  char* currLine = strdup(line);
  //dprintf(2, "CurrLine: %s \n", line);

	char* colonP = strchr(currLine, ':'); //returns location of : or NULL if not found
	*colonP = '\0';
	colonP++;
	curr->targetName = strdup(currLine);
	//colonP actually points to the beginning of the dependencies
	//assign line to dependencies, will parse normally when needed
	curr->dependencies = strdup(colonP);
  /*
  printf("\nTarget Parsed \n");
  dprintf(2, "Name: %s \n", curr->targetName);
	dprintf(2, "Dependencies: %s \n", curr->dependencies);
  */
	return curr;
}
