#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "controlflow.h"
#include "builtin.h"
#include "execute.h"
#include "process.h"

#ifndef _MYSHELL_H_
    #include "myshell.h"
#endif



/********************/
/* helper functions */
/********************/
int ampersand_exist(char **args);
int redirect_out_exist(char **args);



/*
 * purpose: process user command.
 *          take care on control-command (if..then..)
 *                    on buitin-command  (variables, cd..)
 *                    and the regular commands.
 * return: none.
 * */
void process()
{
    extern command *pipeline;
    extern int      cmdno;
    char          **args = pipeline[cmdno].arglist;

    if (args[0] == NULL)
        return;

    /* if '&' at the end, cmd */
    /* will run in backgorund */
    if (ampersand_exist(args))
    {
	extern int dont_wait;
	dont_wait = 1;
    }

    /* if '>' at the end, cmd will */
    /* redirect output to a file   */
    if (redirect_out_exist(args))
    {
        extern int redirect_out;
	redirect_out = 1;
    }

    /* check for changing state for if-statement */
    if (is_control_command(args[0]))
        do_control_command(args);
    
    else if (ok_to_execute())
    {
        if (!builtin_command(args))
	    execute();
    }
}



/********************/
/* helper functions */
/********************/



/*
 * purpose: return 1 if last string is "&".
 *          else return 0.
 * details: free '&' string.
 * */
int ampersand_exist(char **args)
{
    int   i;
    int   rv = 0;
    char *ampersand;

    /* find NULL index */
    for (i = 0; args[i]; ++i)
	;

    if (i > 0 && strcmp(args[i-1], "&") == 0)
    {
        ampersand = args[i-1];
	args[i-1] = NULL;
	free(ampersand);

	rv = 1;
    }

    return rv;
}



/*
 * purpose: return 1 if tow last strings are
 *          ">" and some "filename". else 0.
 * details:  free ">" stirng. "filename"
 *           insert to a global and free later.
 * */
int redirect_out_exist(char **args)
{
    int           i;
    int           rv = 0;
    char        * redirect_sign;
    extern char * redirect_filename;

    /* find NULL index */
    for (i = 0; args[i]; ++i)
        ;
    
    printf("NULL at %d\n", i);
    if (i > 2 && strcmp(args[i-2], ">") == 0)
    {
        redirect_sign     = args[i-2];
	redirect_filename = args[i-1]; /* will free later on main() */
	args[i-2] = NULL;
	free(redirect_sign);

	rv = 1;
    }

    return rv;
}

