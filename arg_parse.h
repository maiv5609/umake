/* tempTarget will be used to store target info without linking to list
 *
 */
typedef struct tempTarget{
	char* targetName; //char* or char of target's name
	char* dependencies;	//line of target's dependencies
} tempTarget;

/* Will take in a char* and return an array of pointers to each argument
 * Will take and seperate line into seperate arguments using whitespace
 */
char** arg_parse(char *line, int *argcp);

/* Will take in line and parse target name and dependencies
 * storing in a tempTarget
 */
tempTarget* target_parse(char *line);




//parse rules for target
//assume lines that come after this block are rules for target
//do not parse arguments for rules
//store whole lines in rules
//when targets are run later do normal parsing and execution for rules
