#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>
#include "target.h"
#include "arg_parse.h"
#include "umake.h"

/* PROTOTYPES
 *
 */

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
			if(dep == 1){
			 //dep check and execution
			 if (list->dependencies == NULL){
				 //no dependencies do nothing
			 }else{
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
		if(list->next == NULL){
			//not found, end of list
			return NULL;
		}else{
		 //not found, return next node
		 return findTargetnode(name, list->next, dep);
	 	}
	}
 }

/*
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
*/

void addNode(char* name, char* dependencies, ruleList* rules){
	targetList* n = (targetList*)malloc(sizeof(targetList));
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
	ruleList* rNode = (ruleList*)malloc(sizeof(ruleList));
	if(!rNode){
		perror("malloc");
		exit(1);
	}
	rNode->currRule = strdup(line);
	rNode->next = NULL;
	if(rList != NULL){
		while(rList->next != NULL){
			rList = rList->next;
		}
		rList->next = rNode;
	}else{
		rList = rNode;
	}
}
