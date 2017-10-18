/* function to read current target's rules
 * accesses current target's rule and returns the rule as a line (char*?)
 * will probably have to loop through until we hit a null pointer on the array
 */
char* getRules(char* currRule);

/* function to get next target in linked list
 * will return null if list is empty or if current target is last one
 */
char* nextTarget()
