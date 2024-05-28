#include <stdio.h>
#include "controlflow.h"
#include "builtin.h"
#include "execute.h"
#include "process.h"

#ifndef _MYSHELL_H_
    #include "myshell.h"
#endif



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

    /* check for changing state for if-statement */
    else if (is_control_command(args[0]))
        do_control_command(args);
    
    else if (ok_to_execute())
    {
        if (!builtin_command(args))
	    execute();
    }
}

