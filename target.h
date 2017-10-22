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
	int size;
}ruleList;

/* Global list of targets
 *
 */
targetList* tList;

/* findTargetnode
 * function to get find indicated target
 * will return null if list is empty or not found in list
 * continues through list until null or target found
 */
void* findTargetnode(char* name, targetList* list);

/* addNode
 * Function used to add a target to the list
 * creates new node and makes this new node the head of the previous list
 */
void addNode(char* name, char* dependencies, ruleList* rules);



/* Collects rules for current target
 * Once a new target is found this list will be copied
 * and attached to a target node
 */
ruleList* rList;
/* addRule
 * Used to add node to rule list if new rule is found
 */
void addRule (char* line);
