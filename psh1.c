/*
 * prompting shell version 1
 *     Prompts for the command and its arguments.
 *     Builds the argument vector for the call to execvp.
 *     Uses execvp(), and never returns
 */
#include <stdio.h>
//#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

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
		execute(arglist);         /* do it      */
		numargs = 0;             /* and reset  */
	    }
	}
    }
}



/*
 * use ecexvp to do it
 */
int execute(char *arglist[])
{
    execvp(arglist[0], arglist); /* do it */
    perror("execvp faild");
    exit(1);
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

