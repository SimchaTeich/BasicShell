/* process2.c
 * command processing layer with builtins
 *
 * The porcess(char **argslist) function is called by the main loop
 * It sits in front of the execute() function. This layer handles
 * two main classes of processing:
 *      a) built-in functions (e.g. exit(), set, =, read, ..)
 *      b) control structures (e.g. if, while, for)
 */

#include <stdio.h>
#include "smsh.h"

int is_control_command(char *s);
int do_control_command(char **args);
int ok_to_execute();
int builtin_command(char **args, int *resultp);

/*
 * purpose: process user command
 * returns: result of processing command
 * details: if a buit-in then call appropriarete function, if not execute()
 *  errors: arise from subroutines, handles there
 * */
int process(char **args)
{
    int rv = 0;

    if (args[0] == NULL)
    {
	rv = 0;
    }
    else if (is_control_command(args[0]))
    {
	rv = do_control_command(args);
    }
    else if (ok_to_execute())
    {
	if (!builtin_command(args, &rv))
	    rv = execute(args);
    }

    return rv;
}

