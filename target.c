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
		 /*
		 dprintf(2, "Target found: %s\n", list->currTarget);
		 dprintf(2, "Dep: %s\n", list->dependencies);
		 dprintf(2, "First rule: %s\n", (list->rules)->currRule);
		 dprintf(2, "Second rule: %s\n\n", ((list->rules)->next)->currRule);
		 */
		 return list->rules;
 	}else{
		if((list->next)->currTarget == NULL){
			//not found, end of list
			return NULL;
		}else{
		 //not found, return next node
		 return findTargetnode(name, list->next, dep);
	 	}
	}
 }

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
	/*
	ruleList* rNode = malloc(sizeof(ruleList));
	if(!rNode){
		perror("malloc");
		exit(1);
	}
	rNode->currRule = strdup(line);
	dprintf(2, "Rule: %s\n", rList->currRule);
	rNode->next = rList;
	rList = list;
 */
 /* https://stackoverflow.com/questions/20418624/how-to-check-if-a-linked-list-is-empty
  *
	*/
	ruleList* rNode = (ruleList*)malloc(sizeof(ruleList));
	rNode->currRule = strdup(line);
	rNode->next = NULL;
	if(rList != NULL){
		while(rList->next != NULL){
			rList = rList->next;
		}
		rList->next = rNode;
	}else{
		//Head of empty list
		rList = rNode;
	}

	//dprintf(2, "Rule 2: %s\n\n", (rList->next)->currRule);
}
