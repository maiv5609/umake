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
		 return list -> rules;
	 }else{
		 //not found, return next node
		 return findTargetnode(name, list->next);
	 }
 }

 /* Function used to add a target to the list
  * creates new node and makes this new node the head of the previous list
  */
void addNode(char* name, char** dependencies, char** rules){
	targetList* n = malloc(sizeof(targetList));
	if(!n){
		perror("malloc");
		exit(1);
	}
	//assignment of variables
	//apprantly Strdupa does the same thing but free memory automatically when you exit the funtion where it was called.
	n -> currTarget = strdup(name); //I might need to free memory here
	n -> dependencies = dependencies;
	n -> next = tList; //adds rest of list to tail
	tList = n;
}
