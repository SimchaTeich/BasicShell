#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "varlib.h"

#ifndef _MYSHELL_H_
    #include "myshell.h"
#endif



/*
 * purpose: run a program.
 * details: using global struct command array,
 *          and global curr-command-index
 *          to run. results saving at the correct
 *          fields in the correct index of the
 *          command array. also, update global last_result.
 * */
void execute()
{
    pid_t            pid;
    extern command * pipeline;
    extern int       cmdno;
    extern int       pipeline_size;
    char           **argv       = pipeline[cmdno].arglist;

    if (argv[0] == NULL)
        return;

    /* create the right pipe for command, except the last.. */
    if (cmdno < pipeline_size)
	pipe(pipeline[cmdno].pipe);

    if ((pid = fork()) == -1)
    {
        perror("fork");
	exit(1);
    }
    else if (pid == 0)
    {
	/* not the first command */
	if (cmdno > 0)
	{
	    close(pipeline[cmdno-1].pipe[1]);
	    dup2(pipeline[cmdno-1].pipe[0], 0);
	    close(pipeline[cmdno-1].pipe[0]);
	}
	
	/* not the last command  */
	if (cmdno < pipeline_size - 1)
	{
	    close(pipeline[cmdno].pipe[0]);
	    dup2(pipeline[cmdno].pipe[1], 1);
	    close(pipeline[cmdno].pipe[1]);
	}

        /* update environ with new non-global variables*/
        extern char **environ;
        environ = VLtable2environ();

	/* enable signals */
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);

	/* and.. Go!      */
        execvp(argv[0], argv);
        perror("cannot execute commands");
        exit(1);
    }
    else
    {
	pipeline[cmdno].pid = pid;

	/* not the first command */
	if (cmdno > 0)
	{
	    close(pipeline[cmdno-1].pipe[0]);
	    close(pipeline[cmdno-1].pipe[1]);
	}

	/* the last command      */
	if (cmdno == pipeline_size - 1)
	{
	    /* wait for all, and save results */ 
	    for (int i = 0; i < pipeline_size; ++i)
		waitpid(pipeline[cmdno].pid, &pipeline[cmdno].result, 0);

	    /* the last result is the result of the pipeline (or only one command..) */
	    extern int last_result;
	    last_result = pipeline[cmdno].result;
	}
    }
}

