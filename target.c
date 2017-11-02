#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "target.h"
#include "arg_parse.h"
#include "umake.h"

/* PROTOTYPES
 *
 */

/* exeDep
 * is passed dependencies and handles calling other functions to
 * execute the dependencies in orders
 */
//exeDep(char* dependencies, targetList* list);

void* findTargetnode(char* name, targetList* list, int dep){
	char** depTargets;
	char* currDep;
	ruleList* currDepRules;
	int argcp = 0;
	int depCounter = 0;
	if(list->currTarget == NULL){
		return NULL;
	}else if (strcmp(list->currTarget, name) == 0){
		 //target found return contents
		 if((list->rules)->size >= 1){
			 if(dep == 1){
				 if (list->dependencies == NULL){
					 //no dependencies do nothing
				 }else{
					 //has dependencies send them somewhere to process each argument as target
					 //call arg_parse to seperate targets
					 //find the targetnode with that target name
					 //store that target name
					 //take name and go through list and execute rules if there are any

					 depTargets = arg_parse(list->dependencies, &argcp);
					 while(depTargets[depCounter] != NULL){
						 currDep = strdup(depTargets[depCounter]);
						 currDepRules = (ruleList *)findTargetnode(currDep, tList, 0);
						 traverseRules(currDepRules);
						 depCounter++;
						 free(currDep);
					 }
				 }
			 }
			 return list->rules;
		 }else{
			 return NULL;
		 }
	 }else{
		 //dprintf(2, "Not found, current node: %s\n", list->currTarget);
		 //dprintf(2, "next node:  %s\n\n", (list->next)->currTarget);

		 if((list->next)->currTarget == NULL){
			 //not found, end of list
			 return NULL;
		 }else {
			 //not found, return next node
			 return findTargetnode(name, list->next, dep);
		 }
	 }
 }

/* Function executes the dependencies that are attached to the current target
 *

 void exeDep(char* dependencies, targetList* list){
	 char* currTarget;
	 char** arguments;
	 ruleList* outputList;
	 //parse the targets from the list of the dependencies


 }
*/
void addNode(char* name, char* dependencies, ruleList* rules){
	targetList* n = malloc(sizeof(targetList));
	if(!n){
		perror("malloc");
		exit(1);
	}
	n->currTarget = strdup(name);
	n->dependencies = dependencies;
	n->rules = rules;
	n->next = tList;
	tList = n;
}

void addRule (char* line){
	ruleList* list = malloc(sizeof(ruleList));
	if(!list){
		perror("malloc");
		exit(1);
	}
	list->currRule = strdup(line);
	list->next = rList;
	rList = list;
	rList->size = (rList->size)+1;
}
