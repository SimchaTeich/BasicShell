/* execute.h
 *     public functions implemented inside execute.c
 */


/*
 * purpose: run a program.
 * details: using global struct command array,
 *          and global curr-command-index
 *          to run. results saving at the correct
 *          fields in the correct index of the
 *          command array. also, update global last_result.
 * */
void execute();
