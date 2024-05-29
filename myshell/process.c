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
int is_amprsnd_at_the_end(char **args);


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
    if (is_amprsnd_at_the_end(args))
    {
	extern int dont_wait;
	dont_wait = 1;
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



/*
 * purpose: return 1 find '&' at the last string.
 *          else return 0.
 * details: free '&' string.
 * */
int is_amprsnd_at_the_end(char **args)
{
    int i;
    int rv = 0;
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

