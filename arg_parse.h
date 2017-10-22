/* tempTarget will be used to store target info without linking to list
 * Will only temporarily hold this information while program finds if
 * this target has rules
 */
typedef struct tempTarget{
	char* targetName;
	char* dependencies;
} tempTarget;

/* arg_parse
 * Takes original pointer and processes it to parse arguments
 * by checking for whitespaces and null terminators inbetween characters
 */
char** arg_parse(char *line, int *argcp);


/* traget_parse
 * arg_parse will use this to parse a line if it detects that it is a target
 * the struct will hold target info while program continues to look for upcoming rules
 */
tempTarget* target_parse(char *line);
