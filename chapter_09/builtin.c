/* builtin.c
 * contains the switch and the functions for builtin commands
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "smsh.h"
#include "varlib.h"

int assign(char *str);
int okname(char *str);

/*
 * purpose: run a builtin command
 * returns: 1 if args[0] is built-in, 0 if not
 * details: test args[0] against all known built-ins. Call functions
 * */
int builtin_command(char **args, int *resultp)
{
    int rv = 0;

    if (strcmp(args[0], "set") == 0)                /* 'set' command? */
    {
        VLlist();
	*resultp = 0;
	rv = 1;
    }
    else if (strchr(args[0], '=') != NULL)          /* assignment cmd */
    {
        *resultp = assign(args[0]);
	if (*resultp != -1)                         /* x-y=123 not ok */
            rv = 1;
    }
    else if (strcmp(args[0], "export") == 0)
    {
        if (args[1] != NULL && okname(args[1]))
	    *resultp = VLexport(args[1]);
	else
	    *resultp = 1;
	
	rv = 1;
    }

    return rv;
}



/*
 * purpose: execute name=val AND ensure that name is legal
 * returns: -1 for illegal lval, or result of VLstore
 * warning: modifies the string, but restore it to normal
 * */
int assign(char *str)
{
    char *cp;
    int  rv;

    cp = strchr(str, '=');
    *cp = '\0';
    rv = (okname(str) ? VLstore(str, cp+1) : -1);
    *cp = '=';

    return rv;
}



/*
 * purpose: determine if a string is a legal variable name
 * returns: 0 for no, 1 for yes
 * */
int okname(char *str)
{
    char *cp;

    for (cp = str; *cp; ++cp)
    {
	if (isdigit(*cp) && cp == str || !(isalnum(*cp) || *cp == '_'))
	    return 0;
    }

    return (cp != str);     /* no empty strings, either */
}

