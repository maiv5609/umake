#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "target.h"

void* findTargetnode(char* name, targetList* list){
	if(list->currTarget == NULL){
		return NULL;
	}else if (strcmp(list->currTarget, name) == 0){
		 //target found return contents
		 if((list->rules)->size >= 1){
			 return list->rules;
		 }else{
			 return NULL;
		 }
	 }else{
		 //not found, return next node
		 return findTargetnode(name, list->next);
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
