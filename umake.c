/* CSCI 347 micro-make * * 09 AUG 2017, Aran Clauson */#include <stdio.h>#include <stdlib.h>#include <unistd.h>#include <sys/wait.h>#include <ctype.h>#include <string.h>#include "arg_parse.h"#include "target.h"#include "umake.h"/* CONSTANTS *//* PROTOTYPES *//* Parse Arguments * returns an array of pointers that point to characters in line *//* parseCommands * Handles parsing command line arguments * makes sure to check that the target being parsed has rules * if not, prints that there are no rules * if target has rules, calls helper function to output */void parseCommands(int argc, const char* argv[]);/* Expand * orig    The input string that may contain variables to be expanded * new     An output buffer that will contain a copy of orig with all *         variables expanded * newsize The size of the buffer pointed to by new. * returns 1 upon success or 0 upon failure. * * Example: "Hello, ${PLACE}" will expand to "Hello, World" when the environment * variable PLACE="World". */int expand(char* orig, char* new, int newsize);/* Process Line * line	 The command line to execute. * This function interprets line as a command line.	It creates a new child * process to execute the line and waits for that process to complete. */void processline(char* line);/* freeLists * Frees rule list and target list */static void freeLists();/* Main entry point. * argc		A count of command-line arguments * argv		The command-line argument valus * * Micro-make (umake) reads from the uMakefile in the current working * directory.	The file is read one line at a time.	Lines with a leading tab * character ('\t') are interpreted as a command and passed to processline minus * the leading tab. */int main(int argc, const char* argv[]) {	FILE* makefile = fopen("./uMakefile", "r");	size_t	bufsize = 0;	char*	 line		= NULL;	char*  eLine = NULL;	ssize_t linelen;	if (makefile == NULL){		fprintf(stderr,"Opening file failed\n");		exit(1);	}else{		linelen = getline(&line, &bufsize, makefile);	}	tempTarget* currTarget = NULL;	char* eAssignCheck = NULL;	char* commentCheck = NULL;	tList = NULL;	rList = NULL;	while(-1 != linelen) {		if(line[linelen-1]=='\n') {			linelen -= 1;			line[linelen] = '\0';		}		//Ignores commented lines		commentCheck = strchr(line, '#');		if (commentCheck != NULL){			*commentCheck = '\0';		}		if(linelen == (size_t)1 || linelen == (size_t)0){		}else{			/* ENVIRONMENT ASSIGNMENT			 *			 */			eLine = strdup(line);			eAssignCheck = strchr(eLine, '=');			if(eAssignCheck != NULL){				env_process(line);				linelen = getline(&line, &bufsize, makefile);			}			//target collection			if(line[0] != '\t'){				if(currTarget != NULL){						//New target found						addNode(currTarget->targetName, currTarget->dependencies, rList);						//resets rule list and parse next target						rList = NULL;						currTarget = target_parse(&line[0]);				}else{					currTarget = target_parse(&line[0]);				}			}else{				addRule(line);			}		}		linelen = getline(&line, &bufsize, makefile);	}	addNode(currTarget->targetName, currTarget->dependencies, rList);	parseCommands(argc, argv);	if(currTarget != NULL){		free(currTarget->targetName);		free(currTarget->dependencies);		free(currTarget);	}	free(line);	free(eLine);	freeLists();	return EXIT_SUCCESS;}static void freeLists() {	targetList* t = tList;	//free targetList	while(t != NULL){		free(t->currTarget);		free(t->dependencies);		targetList* n = t;		t = n->next;		free(n);	}	tList = NULL;	//free global rulelist	ruleList* globalR = rList;	while(globalR != NULL){		free(globalR->currRule);		ruleList* x = globalR;		globalR = x->next;		free(x);	}	rList = NULL;}void parseCommands(int argc, const char* argv[]){	int currArg = 1;	char* currTargetReq;	void* checkList;	ruleList* outputList;	while(argv[currArg]!= '\0'){		currTargetReq = strdup(argv[currArg]);		checkList = findTargetnode(currTargetReq, tList, 1);		if(checkList == NULL){		}else{			//Check passed, it is confirmed that target has rules			outputList = (ruleList *)findTargetnode(currTargetReq, tList, 0);			traverseRules(outputList);		}		//after getting argument pass to findTargetnode and processline		currArg++;		free(currTargetReq);	}} void* traverseRules(ruleList* list){	 	if(list == NULL || list->currRule == NULL){				return NULL;		}		else{				processline(list->currRule);			 	return traverseRules(list->next);		} } int expand(char* orig, char* new, int newsize){	 int patternPosition = 0;	 int patternLength = 0;	 int error = 0;	 char pattern[1024];	 char* currInsert = NULL;	 char* currCheck = NULL;	 size_t bufferLen = 0;	 memset(new, 0, 1024);	 if(orig != NULL){		 currCheck = strdup(orig);		 while(*currCheck != '\0'){			 //Env var detection			 if(*currCheck == '$'){				 currCheck++;				 if(*currCheck == 123){					 error = 1;					 currCheck++;					 while(*currCheck != '\0' && *currCheck != 125){						 pattern[patternLength] = *currCheck;						 patternLength++;						 currCheck++;					 }					 if(*currCheck == 125){						 pattern[patternLength] = '\0';						 currInsert = getenv(pattern);						 if(currInsert == NULL){							 printf("Error: \"%s\" environment variable not found\n", pattern);							 return 0;						 }						 memset(pattern, 0, 1024);						 error = 0;						 currCheck++;					 }				 }else{					 //corrects position if $ was found but not bracket					 currCheck--;				 }			 }			 if(currInsert != NULL){				 //Adding env var				 strcat(new + bufferLen, currInsert);				 bufferLen = strlen(new);				 patternPosition = patternPosition + strlen(currInsert);				 currInsert = NULL;				 patternLength = 0;			 }else{				 //Adding non env strings				 strncat(new, currCheck, 1);				 currCheck++;				 patternPosition++;			 }		 }		 if (error == 1){			 printf("Error: Mismatched brace\n");			 return 0;		 }		 return 1;	 }	 return 0; }/* Process Line * */void processline (char* line) {	char** arguments;	int argcp = 0;	char charBuffer[1024];	int expandCheck = 0;	expandCheck = expand(line, charBuffer, 1024);	if (expandCheck == 1){		arguments = arg_parse(charBuffer, &argcp);	}else{		arguments = arg_parse(line, &argcp);	}	const pid_t cpid = fork();	switch(cpid) {	case -1: {		perror("fork");		break;	}	case 0: {		execvp(arguments[0],arguments);		perror("execvp");		exit(EXIT_FAILURE);		break;	}	default: {		int	 status;		const pid_t pid = wait(&status);		if(-1 == pid) {			perror("wait");		}		else if (pid != cpid) {			fprintf(stderr, "wait: expected process %d, but waited for process %d",							cpid, pid);		}		free(arguments);		break;	}	}}