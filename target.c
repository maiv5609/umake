#include "target.h"


/* Structure that will hold the target's name
 * an array of the target's dependencies, and the target's rules
 */
typedef struct target{
	//char* or char of target's name
	char* currNode;

	//array of target's dependencies
	char** dependencies;

	//array of target's rules
	char** rules;

	//pointer to node of next target (or null if last child)
	char* nextNode;
} target;

/* Function to create new target which would start the linked list
 *
 */


/* Function to add target to next
*
*/


/* Function to read current target's rules
 * accesses current target's rule and returns the rule as a line (char*?)
 * will probably have to loop through until we hit a null pointer on the array
 */
char* getRules(char* currRule){

}

/* function to get next target in linked list
 * will return null if list is empty or if current target is last one
 */
char* nextTarget(){

}
