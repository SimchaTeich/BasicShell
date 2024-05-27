/* execute.h
 *     public functions implemented inside execute.c
 */



/*
 * purpose: run a program passing it arguments
 * returns: status returned via wait, or -1 on error
 *  errors: -1 on fork() or wait() errors
 * */
int execute(char *argv[]);

