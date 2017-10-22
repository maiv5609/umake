#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "target.h"

/* function to get find indicated target
 * will return null if list is empty or not found in list
 * adjusts pointer to whichever node's name matches the one I am searching for
 */
void* findTarget(char* name){
	return findTargetnode(name, tList);
}
/* Helper function for findTarget
 *
 */
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

 /* Function used to add a target to the list
  * creates new node and makes this new node the head of the previous list
  */
void addNode(char* name, char* dependencies, ruleList* rules){
	targetList* n = malloc(sizeof(targetList));
	if(!n){
		perror("malloc");
		exit(1);
	}

	/*
	dprintf(2, "adding 1st Rule: %s \n", rules->currRule);
	dprintf(2, "adding 2nd Rule: %s \n", (rules->next)->currRule);
	*/

	//assignment of variables
	n->currTarget = strdup(name); //I need to free memory here
	n->dependencies = dependencies;
	n->rules = rules;
	n->next = tList; //adds rest of list to tail
	tList = n;

	/*
	if((tList->rules)->currRule != NULL){
		dprintf(2, "accessing 1st Rule: %s \n", (tList->rules)->currRule);
		dprintf(2, "accessing 2nd Rule: %s \n", ((tList->rules)->next)->currRule);
	}
	*/
}

void addRule (char* line){
	ruleList* list = malloc(sizeof(ruleList));
	if(!list){
		perror("malloc");
		exit(1);
	}

	//dprintf(2, "Curr Rule: %s \n", line);
	list->currRule = strdup(line);
	list->next = rList;
	rList = list;
	rList->size = (rList->size)+1;
	//dprintf(2, "Rule added: %s \n", rList->currRule);
}
