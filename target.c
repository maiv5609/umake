#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <time.h>
#include "target.h"
#include "arg_parse.h"
#include "umake.h"

void* findTargetnode(char* name, targetList* list, int dep){
	char** depTargets;
	char* currDep;
	ruleList* currDepRules;
	int argc = 0;
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
				 depTargets = arg_parse(list->dependencies, &argc);
					if(!depCheck(name, depTargets)){
						//returned 0 need to execute rules
						while(depTargets[depCounter] != NULL){
						 currDep = strdup(depTargets[depCounter]);
						 if((ruleList *)findTargetnode(currDep, tList, 1) != NULL){
							 currDepRules = (ruleList *)findTargetnode(currDep, tList, 0);
							 traverseRules(currDepRules);
						 }
						 depCounter++;
						 free(currDep);
					 	}
					}else{
						//do not execute target rules if depCheck fails
						return NULL;
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

int depCheck(char* target, char** dependencies){
	int counter = 0;
	double timeDif = 0.0;
	struct stat targetStat;
	struct stat currDepStat;

	if(stat(target, &targetStat)){
		//target doesn't exist
		return 0;
	}else if(dependencies[0] == NULL){
		return 0;
	}else{
		//target exists check if deps are newer than target
		while(dependencies[counter] != NULL){
			stat(dependencies[counter], &currDepStat);
			timeDif = difftime(targetStat.st_mtime, currDepStat.st_mtime);
			if(timeDif < 0){
				return 0;
			}
			counter++;
		}
		return 1;
	}
	return 1;
}

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
	if(rList == NULL){
		rList = rNode;
	}else{
		ruleList* current = rList;
		while(current->next != NULL){
			current = current->next;
		}
		current->next = rNode;
	}

}
