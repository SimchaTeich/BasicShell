/*
 * prompting shell version 2
 *     Solves the 'one-shot' problem of version 1
 *         Uses execvp(), buf fork()s first so that the
 *         shell waits around to perform another command
 *     New problem: shell ctaches signals. Run vi, press ^C.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define MAXARGS 20            /* cmdline args */
#define ARGLEN 100            /* token length */

char *makestring(char *buf);
int execute(char *arglist[]);

int main()
{
    char *arglist[MAXARGS+1]; /* an array of ptrs */
    int numargs;              /* insex into array */
    char argbuf[ARGLEN];      /* read stufff here */

    numargs = 0;
    while (numargs < MAXARGS)
    {
        printf("Arg[%d]? ", numargs);
	if (fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n')
	{
	    arglist[numargs] = makestring(argbuf);
	    ++numargs;
	}
	else
	{
	    if (numargs > 0)             /* any args?  */
	    {
	        arglist[numargs] = NULL; /* close list */
		execute(arglist);        /* do it      */
		numargs = 0;             /* and reset  */
	    }
	}
    }
}



/*
 * use fork and execvp and wait to do it
 */
int execute(char *arglist[])
{
    pid_t pid;                           /* of child */
    int exitstatus;                      /* of child */

    pid = fork();
    switch(pid)
    {
        case -1:
	    perror("fork failed");
	    exit(1);
	    break;
	
	case 0:
	    execvp(arglist[0], arglist);  /* do it */
	    perror("execvp faild");
	    exit(1);
	    break;
	
	default:
	    signal(SIGINT, SIG_IGN);      /* protect psh2 from SIGINT from child procces */
	    while (wait(&exitstatus) != pid)
		;
	    printf("child exited with status %d, %d\n", exitstatus>>8, exitstatus&0377);
	    signal(SIGINT, SIG_DFL);      /* now, until the next command, user can say goodbay */
	    break;

    }
}



/*
 * trim off newline and create storage for the string
 */
char *makestring(char *buf)
{
    char *cp;

    buf[strlen(buf)-1] = '\0';          /* trim newline */
    cp = (char *)malloc(strlen(buf)+1); /* get memory   */
    if (!cp)                            /* or die       */
    {
        fprintf(stderr, "no memory\n");
	exit(1);
    }

    strcpy(cp, buf);                    /* copy chars */
    return cp;                          /* return ptr */
}

