/* Target node
 * Rules will be held in a linked list that is attached to each target
 */
typedef struct targetList{
	char* currTarget;
	char* dependencies;
	struct ruleList* rules;
	struct targetList* next;
} targetList;

/* Rule Node
 *
 */
typedef struct ruleList{
	char* currRule;
	struct ruleList* next;
}ruleList;

/* Global list of targets
 *
 */
targetList* tList;


/* depCheck
 * takes in target name and its dependencies
 * checks if target doesnt exist or if any of the dependency files are newer
 * returns 0 if rules need to be executed, 1 otherwise
 */
int depCheck(char* target, char** dependencies);

/* findTargetnode
 * function to get find indicated target
 * if dep is 1 then function will check for dependencies
 * If a target has dependencies then it will call a function
 * to handle those dependencies
 */
void* findTargetnode(char* name, targetList* list, int dep);

/* addNode
 * Function used to add a target to the list
 */
void addNode(char* name, char* dependencies, ruleList* rules);


/* Collects rules for current target
 *
 */
ruleList* rList;

/* addRule
 * Used to add node to rule list if new rule is found
 */
void addRule (char* line);
