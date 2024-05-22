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








