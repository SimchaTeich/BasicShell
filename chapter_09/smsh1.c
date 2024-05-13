/**  smsh1.c  small-shell version1
 **             first really usefull version after prompting shell
 **             this one parses the command line into strings
 **             use fork, exec, wait, and ignore signals
 **/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "smsh.h"

#define DFL_PROMPT "> "

void setup();
void fatal(char *s1, char *s2, int n);

int main()
{
    char *cmdline, *prompt, **arglist;
    int   result;

    prompt = DFL_PROMPT;
    setup();

    while ((cmdline = next_cmd(prompt, stdin)) != NULL)
    {
        if ((arglist = splitline(cmdline)) != NULL)
	{
	    result = execute(arglist);
	    freelist(arglist);
	}
	free(cmdline);
    }

    return 0;
}



/*
 * purpose: initialize shell
 * returns: nothing. calls fatal() if trouble
 * */
void setup()
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}



void fatal(char *s1, char *s2, int n)
{
    fprintf(stderr, "Error: %s, %s\n", s1, s2);
    exit(n);
}

