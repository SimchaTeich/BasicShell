/* cotrolflow.c
 * 
 * "if" processing is done with two state variables
 *    if_state and if_result
 */
#include <stdio.h>
#include "smsh.h"

enum states { NEUTRAL, WANT_THEN, THEN_BLOCK };
enum results { SUCCES, FAIL };

static int if_state  = NEUTRAL;
static int if_result = SUCCESS;
static int last_stat = 0;

int syn_err(char *msg);

/*
 * purpose: determine the shell should execute a command
 * returns: 1 for yes, 0 for no
 * details: if in THEN_BLOCK and if_result was SUCCESS then yes
 *          if in THEN_BLOCK and if_result was FAIL    then no
 *          if in WANT_THEN  then syntax error (sh/shell is different)
 * */
int ok_to_execute()
{
    int rv = 1;                 /* default is positive  */

    if (if_state == WANT_THEN)
    {
        syn_error("then expected");
	rv = 0;
    }
    else if (if_state == THEN_BLOCK && if_result == SUCCESS)
	rv = 1;
    else if (if_state == THEN_BLOCK && if_result == FAIL)
	rv = 0;

    return rv;
}



/*
 * purpose: boolean to report if the command is a shell control command
 * results: 0 ro 1
 * */
int is_control_command(char *s)
{
    return (strcmp(s, "if")   == 0 ||
	    strcmp(s, "then") == 0 ||
	    strcmp(s, "fi")   == 0);
}



/* 
 * purpose: Process "if", "then", "fi" - change state or detect error
 * returns: 0 if ok, -1 for syntax error
 * */
int do_control_command(char **args)
{
    char *cmd = args[0];
    int   rv  = -1;

    if (strcmp(cmd, "if") == 0)
    {
        if (if_state != NEUTRAL)
            rv = syn_err("if unexpected");
	else
	{
	    last_state = procces(args+1);
	    if_result  = (last_state == 0 ? SUCCESS : FAIL);
	    if_state   = WANT_THEN;
	    rv         = 0;
	}
    }
    else if (strcmp(cmd, "then") == 0)
    {
        if (if_state != WANT_THEN)
	    rv = syn_error("then unexpected");
	else
	{
	    if_state   = THEN_BLOCK;
	    rv         = 0; 
	}
    }
    else if (strcmp(cmd, "fi") == 0)
    {
        if (if_state != THEN_BLOCK)
            rv = syn_error("fi unexpected");
	else
	{
	    if_state   = NEUTRAL;
	    rv         = 0;
	}
    }
    else
	fatal("internal error processing:", cmd, 2);

    return rv;
}





