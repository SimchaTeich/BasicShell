#include <stdio.h>
#include "controlflow.h"
#include "builtin.h"
#include "execute.h"
#include "process.h"

#ifndef _MYSHELL_H_
    #include "myshell.h"
#endif

/*
 * purpose: process user command
 * returns: result of processing command
 * details: if a buit-in then call appropriarete function,
 *          if not execute()
 *  errors: arise from subroutines, handles there
 * */
void process()
{
    extern command *pipeline;
    extern int      cmdno;
    char          **args    = pipeline[cmdno].arglist;
    int             rv      = 0;

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
            //rv = execute();
	    execute();
    }

    //return rv;
}

