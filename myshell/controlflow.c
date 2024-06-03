#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "controlflow.h"

#ifndef _MYSHELL_H_
    #include "myshell.h"
#endif



/*********************/
/* helper functions  */
/*********************/
int syn_err(char *msg);
void delete_if_word(char *cmd);

static int if_state  = NEUTRAL;


/*
 * purpose: determine the shell should execute a command
 * returns: 1 for yes, 0 for no
 * details: if in THEN_BLOCK and if_result was SUCCESS then yes
 *          if in THEN_BLOCK and if_result was FAIL    then no
 *          if in WANT_THEN  then syntax error (sh/shell is different)
 * */
int ok_to_execute()
{
    extern int cmdno;
    extern int last_result;
    int        rv = 1;                       /* default is positive  */

    if (if_state == WANT_THEN && cmdno == 0) /* if cmdno > 0, its inside pipeline.. */
    {
        syn_err("then expected");
	rv = 0;
    }
    else if (if_state == THEN_BLOCK && last_result == SUCCESS)
	rv = 1;
    else if (if_state == THEN_BLOCK && last_result != SUCCESS)
	rv = 0;
    else if (if_state == ELSE_BLOCK && last_result == SUCCESS)
	rv = 0;
    else if (if_state == ELSE_BLOCK && last_result != SUCCESS)
	rv = 1;

    return rv;
}



/*
 * purpose: boolean to report if the command is
 *          a shell control command
 * results: 0 ro 1
 * */
int is_control_command(char *s)
{
    return (strcmp(s, "if")   == 0 ||
	    strcmp(s, "then") == 0 ||
	    strcmp(s, "else") == 0 ||
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
	    delete_if_word(cmd); 
	    process();
	    if_state   = WANT_THEN;
	    rv         = 0;
	}
    }
    else if (strcmp(cmd, "then") == 0)
    {
        if (if_state != WANT_THEN)
	    rv = syn_err("then unexpected");
	else
	{
	    if_state   = THEN_BLOCK;
	    rv         = 0; 
	}
    }
    else if (strcmp(cmd, "else") == 0)
    {
        if (if_state != THEN_BLOCK)
	    rv = syn_err("else unexpected");
	else
	{
	    if_state   = ELSE_BLOCK;
	    rv         = 0;
	}
    }
    else if (strcmp(cmd, "fi") == 0)
    {
        if (if_state != ELSE_BLOCK &&
	    if_state != THEN_BLOCK   )
            rv = syn_err("fi unexpected");
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



/**********************************/
/*        helper functions        */
/**********************************/



/*
 * purpose: handles syntax error in control structures
 * details: reset state to NEUTRAL
 * returns: -1 in interactive mode. Shouls call fatal in scripts
 * */
int syn_err(char *msg)
{
    if_state = NEUTRAL;
    fprintf(stderr, "syntax error: %s\n", msg);
    return -1;
}



/*
 * purpose: delete the word 'if' from the command
 * details: move every string left in the arglist
 *          of pipeline[cmdno].
 *          then, free the "if" string.
 * */
void delete_if_word(char *cmd)
{
    extern command *pipeline;
    extern int       cmdno;
    int j;
    
    for (j = 1; pipeline[cmdno].arglist[j]; ++j)
        pipeline[cmdno].arglist[j-1] = pipeline[cmdno].arglist[j];
    pipeline[cmdno].arglist[j-1] = NULL;
    
    free(cmd);
}


