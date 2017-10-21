

/* Structure that will hold the target's name
 * an array of the target's dependencies, and the target's rules
 */
typedef struct targetList{
	char* currTarget; //char* or char of target's name
	char* dependencies;	//line of target's dependencies
	struct ruleList* rules;	//target's rules
	struct targetList* next;	//pointer to node of next target (or null if last child)
} targetList;

/* Structure will hold rules in a linked list
 *
 */
typedef struct ruleList{
	char* currRule;
	struct ruleList* next;
}ruleList;

//tList is the global list of targets that I am adjusting
targetList* tList;

//collects rules for current target
ruleList* rList; 


void addRule (char* line);

/* function to read current target's rules
 * accesses current target's rule and returns the rule as a line (char*?)
 * will probably have to loop through until we hit a null pointer on the array
 */
char* getRules(char* currRule);
void* findTarget(char* name);
void* findTargetnode(char* name, targetList* list);
void addNode(char* name, char* dependencies, ruleList* rules);
