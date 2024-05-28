/* builtin.h
 *     public functions implemented inside builtin.c
 */



/*
 * purpose: run a builtin command
 * returns: 1 if args[0] is built-in, 0 if not
 * details: test args[0] against all known built-ins. Call functions
 * */
int builtin_command(char **args);//, int *resultp);

