/* CSCI 347 micro-make * * 09 AUG 2017, Aran Clauson */#include <stdio.h>#include <stdlib.h>#include <unistd.h>#include <sys/wait.h>#include <ctype.h>#include <string.h>#include "arg_parse.h"#include "target.h"#include "umake.h"/*	WORKNOTE: work on issue 9 first, execute target dependencies and follow them in order	good information on makefiles: https://www.codeproject.com/Articles/31488/Makefiles-in-Linux-An-Overview*//* CONSTANTS *//* PROTOTYPES *//* Parse Arguments * returns an array of pointers that point to characters in line *//* parseCommands * Handles parsing command line arguments * makes sure to check that the target being parsed has rules * if not, prints that there are no rules * if target has rules, calls helper function to output */void parseCommands(int argc, const char* argv[]);/* Expand * orig    The input string that may contain variables to be expanded * new     An output buffer that will contain a copy of orig with all *         variables expanded * newsize The size of the buffer pointed to by new. * returns 1 upon success or 0 upon failure. * * Example: "Hello, ${PLACE}" will expand to "Hello, World" when the environment * variable PLACE="World". */int expand(char* orig, char* new, int newsize);/* Process Line * line	 The command line to execute. * This function interprets line as a command line.	It creates a new child * process to execute the line and waits for that process to complete. */void processline(char* line);/* Main entry point. * argc		A count of command-line arguments * argv		The command-line argument valus * * Micro-make (umake) reads from the uMakefile in the current working * directory.	The file is read one line at a time.	Lines with a leading tab * character ('\t') are interpreted as a command and passed to processline minus * the leading tab. * * Main Adjustments made by student: * Program now runs in two modes, target mode and argument mode. Depending on the * first line of the file it will determine whether or not to parse for targets. * Once it is parsing for targets it will look for a : on the current line. * If it finds one it will split and store the current line's (target) information * temporarily while it continues to look if the target has rules. * If another target is found it assumes the previous target didn't have rules and * adds it to the linked list. * If a rule(s) is found it creates a node in a rule list. If a node was created * previously and a new target is found the program will copy the rule list to * a target node created using the previous target's information and adds * to global list. * * Many checks were added to handle different use cases. */int main(int argc, const char* argv[]) {	FILE* makefile = fopen("./uMakefile", "r");	size_t	bufsize = 0;	char*	 line		= NULL;	char*  eLine = NULL;	ssize_t linelen = getline(&line, &bufsize, makefile);	tempTarget* currTarget = NULL;	char* eAssignCheck = NULL;	//WORKNOTE: I need to take both of these lists and set them to null. I dont need to malloc because when I did I basically just created a blank space in memory that was added onto the front of my list and does nothing.	//I ended up executing other target's rules because rulelist is being linked to other targets because free() and malloc right after doesnt do anything.	//Also I didnt need to keep the previous assignments functionality and only need to parse targets.	//Also double check what I didn with size because at this point it doesnt really do anything because it was never initialized to anything	//Also rule list doesnt need to be global, only target list.	//Make sure to go through and correctly free memory	//Use this to detect memory leaks and other issues	//valgrind --leak-check=yes --track-origins=yes ./umake B C	//http://valgrind.org/docs/manual/quick-start.html	tList = NULL;	rList = NULL;	while(-1 != linelen) {		if(line[linelen-1]=='\n') {			linelen -= 1;			line[linelen] = '\0';		}		//dprintf(2, "Current Line: %s\n\n",line);		/* ENVIRONMENT ASSIGNMENT		 *		 */		eLine = strdup(line);		eAssignCheck = strchr(eLine, '=');		if(eAssignCheck != NULL){			//dprintf(2, "Line: %s\n", line);			//do Assignment			env_process(line);			linelen = getline(&line, &bufsize, makefile);		}		//target collection		if(line[0] != '\t'){			if(currTarget != NULL){					/*					dprintf(2, "Target Added: %s\n", currTarget->targetName);					dprintf(2, "Dep: %s\n", currTarget->dependencies);					dprintf(2, "Rule 1: %s\n", rList->currRule);					dprintf(2, "Rule 2: %s\n\n", (rList->next)->currRule);					*/					//New target found					addNode(currTarget->targetName, currTarget->dependencies, rList);					//resets rule list and parse next target					rList = NULL;					currTarget = target_parse(&line[0]);			}else{				currTarget = target_parse(&line[0]);				//dprintf(2, "Target found: %s\n", currTarget->targetName);			}		}else{			//rule collection			addRule(line);		}		//next line		linelen = getline(&line, &bufsize, makefile);	}	//I probably dont have to set rList to null and can probably just go free everything	rList = NULL;	/* Test print of target lists	 *	 */	parseCommands(argc, argv);	/* WORKNOTE: this doesnt free all the memory that was allocated,   * I need to loop through my targets, free each string and loop	 * through their rule lists and free those as well.	 */	 /*		free(currTarget->targetName);		free(currTarget->dependencies);		free(currTarget);		free(tList->currTarget);		free(rList->currRule);	free(tList);	free(rList);	free(line);	free(eLine);	*/	return EXIT_SUCCESS;}void parseCommands(int argc, const char* argv[]){	int currArg = 1;	char* currTargetReq;	void* checkList;	ruleList* outputList;	while(argv[currArg]!= '\0'){		currTargetReq = strdup(argv[currArg]);		//Check and execute dependencies		checkList = findTargetnode(currTargetReq, tList, 1);		if(checkList == NULL){		}else{			//Check passed, it is confirmed that target has rules			outputList = (ruleList *)findTargetnode(currTargetReq, tList, 0);			traverseRules(outputList);		}		//after getting argument pass to findTargetnode and processline		currArg++;		free(currTargetReq);	}} void* traverseRules(ruleList* list){	 	if(list == NULL || list->currRule == NULL){				return NULL;		}		else{				processline(list->currRule);			 	return traverseRules(list->next);		} } /* Expand  * orig    The input string that may contain variables to be expanded  * new     An output buffer that will contain a copy of orig with all  *         variables expanded  * newsize The size of the buffer pointed to by new.  * returns 1 upon success or 0 upon failure.  *  * Example: "Hello, ${PLACE}" will expand to "Hello, World" when the environment  * variable PLACE="World".  */	//Currently assumes that there will only be one expansion int expand(char* orig, char* new, int newsize){	 /*	 *create a char** buffer that is similar in size to line	 *Iterate through the orignal string	 *As soon as you find ${ begin storing the incoming characters until you hit a }	 *		if during this time you hit a \0 then return an error in syntax to the user	 *After finding a closing bracket send the stored string through getenv() to find the *associated *value with it.	 * take note of the position of $ and use it to insert the value of the environment variable after you parse the variable	 *Will only do one pass as it even if it replaces it with another env	 */	 //123 is a beginning bracket	 //125 is a closing bracket	 //use this to track position of insertion	 int patternPosition = 0;	 int patternLength = 0;	 char pattern[1024];	 char* currInsert;	 if(orig != NULL){		 new = strdup(orig);		 //dprintf(2, "Expanding: %s\n", orig);		 //dprintf(2, "Copy: %s\n\n", new);		 while(*orig != '\0'){			 if(*orig == '$'){				 orig++;				 if(*orig == 123){					 orig++;					 while(*orig != '\0' && *orig != 125){						 //dprintf(2, "Found: %c\n", *orig);						 pattern[patternLength] = *orig;						 //dprintf(2, "Inserted at %d: %c\n\n", patternLength, pattern[patternLength]);						 patternLength++;						 orig++;					 }					 if(*orig == 125){						 pattern[patternLength] = '\0';						 currInsert = getenv(pattern);						 //dprintf(2, "%s: %s\n\n", pattern, currInsert);						 //clear out pattern buffer						 for (int clear = 0; clear <= patternLength; clear++){							 pattern[patternLength] = 0;						 }						 //reset pattern counter						 patternLength = 0;						 //end of pattern do insertion						 dprintf(2, "patternPosition: %d\n", patternPosition);					 }				 }else{					 //corrects position if $ was found but not bracket					 orig--;				 }			 }			 dprintf(2, "Curr: %c\n", *orig);			 orig++;			 patternPosition++;		 }		 return 1;	 }	 return 0; }/* Process Line * */void processline (char* line) {	char** arguments;	int argcp = 0;	char charBuffer[1024];	int expandCheck = 0;	//Call expand here before this to adjust the arguments	expandCheck = expand(line, charBuffer, 1024);	/*	if (expandCheck == 1){		arguments = arg_parse(charBuffer, &argcp);	}else{		arguments = arg_parse(line, &argcp);	}	*/	arguments = arg_parse(line, &argcp);	const pid_t cpid = fork();	switch(cpid) {	case -1: {		perror("fork");		break;	}	case 0: {		execvp(arguments[0],arguments);		perror("execvp");		exit(EXIT_FAILURE);		break;	}	default: {		int	 status;		const pid_t pid = wait(&status);		if(-1 == pid) {			perror("wait");		}		else if (pid != cpid) {			fprintf(stderr, "wait: expected process %d, but waited for process %d",							cpid, pid);		}		free(arguments);		break;	}	}}