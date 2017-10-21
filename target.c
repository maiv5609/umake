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
		 ruleList* current = &list;

		 //char* currentRule = current->currRule;

		 /*
		 dprintf(2, "Name: %s \n", name);
		 dprintf(2, "Dependencies: %s \n", list->dependencies);
		 dprintf(2, "Rules: %s \n \n", rList->currRule);
		 */
		 //https://stackoverflow.com/questions/23178741/access-members-of-a-structure-from-void-pointer-inside-a-structure
		 //This page might detail how to handle looking at a value inside a nested struct

		 dprintf(2, "Rules: %s \n \n", ((struct ruleList *)current.rules->currRule));
		 return list->rules;
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
	//assignment of variables
	n->currTarget = strdup(name); //I need to free memory here
	n->dependencies = dependencies;
	n->rules = rules;
	n->next = tList; //adds rest of list to tail
	tList = n;
}

void addRule (char* line){
	ruleList* list = malloc(sizeof(ruleList));
	if(!list){
		perror("malloc");
		exit(1);
	}
	dprintf(2, "Curr Rule: %s \n", line);
	list->currRule = strdup(line); //seg faults
	list->next = rList;
	rList = list;

}
