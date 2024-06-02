/* builtin.c
 *     contains the switch and the functions for builtin commands
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include "splitline.h"
#include "varlib.h"



/********************/
/* helper functions */
/********************/
int assign(char *var, char *val);
int okname(char *str);
int read_arg(char *var);



/*
 * purpose: run a builtin command (variables, cd..)
 * returns: 1 if args[0] is built-in, 0 if not
 * details: test args[0] against all known built-ins. Call functions
 * */
int builtin_command(char **args)
{
    int rv = 0;

    /* about variables */
    if (strcmp(args[0], "set") == 0)                /* 'set' command? */
    {
        VLlist();
	rv = 1;
    }
    else if (args[0][0] == '$' && args[1] && strcmp(args[1], "=") == 0 && args[2] != NULL)   /* assignment cmd. very stupid. just for H.M... */
    {
	if (assign(args[0], args[2]) != -1)                  /* x-y=123 not ok */
            rv = 1;
    }
    else if (strcmp(args[0], "export") == 0)
    {
        if (args[1] != NULL && okname(args[1]))
	    VLexport(args[1]);
	
	rv = 1;
    }
    else if (strcmp(args[0], "read") == 0)
    {
        if (args[1] != NULL && read_arg(args[1]) != -1)
	    rv = 1;
    }

    /* or change the prompt sign?  */
    else if (strcmp(args[0], "prompt") == 0)
    {
        if (strcmp(args[1], "=") == 0 &&
	    args[2] != NULL           &&
	    args[3] == NULL             )
	{
	    extern char *prompt;
	    free(prompt);
	    prompt = newstr(args[2], strlen(args[2])+1); /* +1 for ' ' */
	    prompt[strlen(args[2])] = ' ';
	}

	rv = 1;
    }

    /* or change current directory? */
    else if (strcmp(args[0], "cd") == 0)
    {
        if (args[1] != NULL &&
            args[2] == NULL   )
	{
	    chdir(args[1]);
	}

	rv = 1;
    }

    /* or quit */
    else if (strcmp(args[0], "quit") == 0)
    {
        if (args[1] == NULL)
	{
	    extern int quit;
	    quit = 1;
	}

	rv = 1;
    }

    return rv;
}



/**************************************/
/*          helper functions          */
/**************************************/



/*
 * purpose: execute name=val AND ensure that name is legal
 * returns: -1 for illegal lval, or result of VLstore
 * warning: modifies the string, but restore it to normal
 * */
int assign(char *var, char *val) //assign(str)
{
    //char *cp;
    int  rv;

    //cp = strchr(str, '=');
    //*cp = '\0';
    rv = (okname(var+1) ? VLstore(var+1, val) : -1); /* +1 because of the '$'*/
    //*cp = '=';

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



/*
 * purpose: read argument value from user
 *          and insert it to the variables list.
 * */
int read_arg(char *var)
{
    int  rv;
    char val[50];

    fgets(val, 50, stdin);
    val[strlen(val)-1] = '\0';
    
    rv = (okname(var) ? VLstore(var, val) : -1);

    return rv;
}
