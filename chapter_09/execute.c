/* execute.c - code used by small shell to execute commands */

#include <stdio.h>
#include <srdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

/*
 * purpose: run a program passing it arguments
 * returns: status returned via wait, or -1 on error
 *  errors: -1 on fork() or wait() errors
 * */
int execute(char *argv[])
{
    pid_t pid;
    int child_info = -1;

    if (argv[0] == NULL)                /* nothing succeeds */
	return 0;

    if ((pid = fork()) == -1)
    {
	perror("fork");
    }
    else if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execvp(argv[0], argv);
	perror("cannot execute commands");
	exit(1);
    }
    else
    {
        if (wait(&child_info) == -1)
	    perror("wait");
    }

    return child_info;
}

