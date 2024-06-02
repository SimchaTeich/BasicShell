#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "controlflow.h"
#include "builtin.h"
#include "execute.h"
#include "process.h"
#include "varlib.h"
#include "splitline.h"

#ifndef _MYSHELL_H_
    #include "myshell.h"
#endif



/********************/
/* helper functions */
/********************/
int  ampersand_exist(char **args);
int  redirect_out_exist(char **args);
int  redirect_outa_exist(char **args);
int  redirect_err_exist(char **args);
void replace_dolars(char **args);


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
    if (ampersand_exist(args))
    {
	extern int dont_wait;
	dont_wait = 1;
    }

    /* if '>' at the end, cmd will   */
    /* redirect output to a file     */
    if (redirect_out_exist(args))
    {
        extern int redirect_out;
	redirect_out = 1;
    }
    /* or '>>', using to add to file */
    else if (redirect_outa_exist(args))
    {
        extern int redirect_outa;
	redirect_outa = 1;
    }
    /* or maybe redirect the stderr  */
    else if (redirect_err_exist(args))
    {
        extern int redirect_err;
	redirect_err = 1;
    }
    

    /* check for changing state for if-statement */
    if (is_control_command(args[0]))
        do_control_command(args);
    
    else if (ok_to_execute())
    {
	replace_dolars(args);

        if (!builtin_command(args))
	{
	    execute();
	}
    }
}



/********************/
/* helper functions */
/********************/



/*
 * purpose: return 1 if last string is "&".
 *          else return 0.
 * details: free '&' string.
 * */
int ampersand_exist(char **args)
{
    int   i;
    int   rv = 0;
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



/*
 * purpose: return 1 if tow last strings are
 *          ">" and some "filename". else 0.
 * details:  free ">" string. "filename"
 *           insert to a global and free later.
 * */
int redirect_out_exist(char **args)
{
    int           i;
    int           rv = 0;
    char        * redirect_sign;
    extern char * redirect_filename;

    /* find NULL index */
    for (i = 0; args[i]; ++i)
        ;
    
    if (i > 2 && strcmp(args[i-2], ">") == 0)
    {
        redirect_sign     = args[i-2];
	redirect_filename = args[i-1]; /* will free later on main() */
	args[i-2] = NULL;
	free(redirect_sign);

	rv = 1;
    }

    return rv;
}



/*
 * purpose: return 1 if tow last strings are
 *          ">>" and some "filename". else 0.
 * details:  free ">>" string. "filename"
 *           insert to a global and free later.
 * */
int redirect_outa_exist(char **args)
{
    int           i;
    int           rv = 0;
    char        * redirect_sign;
    extern char * redirect_filename;

    /* find NULL index */
    for (i = 0; args[i]; ++i)
        ;

    if (i > 2 && strcmp(args[i-2], ">>") == 0)
    {
        redirect_sign     = args[i-2];
        redirect_filename = args[i-1]; /* will free later on main() */
        args[i-2] = NULL;
        free(redirect_sign);

        rv = 1;
    }

    return rv;
}



/*
 * purpose: return 1 if tow last strings are
 *          "2>" and some "filename". else 0.
 * details:  free "2>" string. "filename"
 *           insert to a global and free later.
 * */
int redirect_err_exist(char **args)
{
    int           i;
    int           rv = 0;
    char        * redirect_sign;
    extern char * redirect_filename;

    /* find NULL index */
    for (i = 0; args[i]; ++i)
        ;

    if (i > 2 && strcmp(args[i-2], "2>") == 0)
    {
        redirect_sign     = args[i-2];
        redirect_filename = args[i-1]; /* will free later on main() */
        args[i-2] = NULL;
        free(redirect_sign);

        rv = 1;
    }

    return rv;
}



/*
 * purpose: repalce every "$var" in args
 *          with the correct "val", when
 *          $var=val. if "$?", replace it
 *          with the last command result.
 * details: using varlib.h
 * */
void replace_dolars(char **args)
{
    int   i = 1;
    char *arg;
    char *var;

    if (args[0][0] == '$' && strlen(args[0]) > 1 && (args[1] && args[1][0] != '=' || args[1] == NULL))
	i = 0;

    for (; args[i]; ++i) /* never replace the first (very stupid, but OK for my H.M...) */
    {
	if (args[i][0] == '$')
	{
	    if (strcmp(args[i], "$?") == 0)
	    {
	        extern int last_result;
                free(args[i]);
		args[i] = emalloc(5);
		sprintf(args[i], "%d", last_result);
		continue;
	    }

            var = VLlookup(args[i]+1);                /* +1 because name start after the "$" */
	    if (strcmp(var, "") != 0)
	    {
		arg = args[i];
	        args[i] = newstr(var, strlen(var));
		free(arg);
	    }
        }
    }
}

