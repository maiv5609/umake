#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "target.h"

/*
page about invalid type argument '->' error
I am probably declaring someone incorrectly
https://stackoverflow.com/questions/4983010/invalid-type-argument-of-c-structs
*/


/* Structure that will hold the target's name
 * an array of the target's dependencies, and the target's rules
 */
typedef struct targetList{
	char* currTarget; //char* or char of target's name
	char** dependencies;	//array of target's dependencies
	char** rules;	//array of target's rules
	struct targetList* next;	//pointer to node of next target (or null if last child)
} targetList;

/* PROTOTYPES */
void* findTarget(char* name);
void* findTargetnode(char* name, targetList list);
void addNode(char* name, char** dependencies, char** rules);

//tList is the global list of targets that I am adjusting
targetList tList;


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
void* findTargetnode(char* name, targetList list){
	if(*list->currTarget == NULL){
		return NULL;
	}else if (strcmp(*list->currTarget, name) == 0){
		 //target found return contents
		 return list -> rules;
	 }else{
		 //not found, return next node
		 return findTargetnode(name, list->next);
	 }
 }

 /* Function used to add a target to the list
  * Use this to build list
  */
void addNode(char* name, char** dependencies, char** rules){
	targetList n = malloc(sizeof(targetList));
	if(!n){
		perror("malloc");
		exit(1);
	}
	//assignment of variables
	//apprantly Strdupa does the same thing but free memory automatically when you exit the funtion where it was called.
	n -> currTarget = Strdup(name); //I might need to free memory here
	n -> dependencies = dependencies;
	n -> next = tList;
	tList = n;
}
