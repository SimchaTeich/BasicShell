#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myshell.h"    /* using fatal() */
#include "splitline.h"
#include "conio.h"
#include <readline/readline.h>
#include <readline/history.h>

#define is_delim(x) ((x)==' ' || (x)=='\t')


int up_arrow(int ,int);
int down_arrow(int ,int);

static int history_index = -1;
extern history hist;
extern char * prompt;

int first_time = 1;


/*
 * purpose: read next command line from fp
 * returns: dynamiclly allocated string holding command line
 *  errors: NULL at EOF (not really an error)
 *          calls fatal from emalloc()
 *   notes: allocates space in BUFSIZ chunks.
 * */
char *next_cmd(char *prompt, FILE *fp)
{
    char *buf;                            /* the buffer           */
    history_index = -1;                   /* start from the begining evey time */

    rl_bind_keyseq("\\e[A", up_arrow);
    rl_bind_keyseq("\\e[B", down_arrow);

    buf = readline(prompt);                /* prompt user  */

    if (buf != NULL && strcmp(buf, "\033[A") != 0 && strcmp(buf, "\033[B") != 0)
    {
	history_index = -1;
	first_time = 1;
    }

    return buf;
}



/*
 * purpose: split a line into array of white-space seperated tokens
 * returns: a NULL-terminated array of pointers to copies of the tokens
 *          or NULL if line if no tokens on the line
 *  action: traverse the array, locate strings, make copies
 *    note: strtok() could work, but we may want to add quotes later
 * */
char ** splitline(char *line)
{
    char **args;
    int    spots    = 0;                  /* spots in table       */
    int    bufspace = 0;                  /* bytes in table       */
    int    argnum   = 0;                  /* slots used           */
    char  *cp       = line;               /* pos in string        */
    char  *start;
    int    len;

    if (line == NULL)                     /* handle special case  */
        return NULL;

    args     = emalloc(BUFSIZ);           /* initialize array     */
    bufspace = BUFSIZ;
    spots    = BUFSIZ/sizeof(char *);

    while (*cp != '\0')
    {
        while (is_delim(*cp))             /* skip leading spaces  */
            ++cp;

	if (*cp == '\0')                  /* quit at end-o-string */
	    break;

	/* make sure the array has room (+1 for NULL) */
	if (argnum + 1 > spots)
	{
	    args = erealloc(args, bufspace + BUFSIZ);
	    bufspace += BUFSIZ;
	    spots += (BUFSIZ/sizeof(char *));
	}

	/* mark start, then find end of word */
	start = cp;
	len   = 1;
	while (*++cp != '\0' && !(is_delim(*cp)))
	    ++len;

	args[argnum] = newstr(start, len);
	++argnum;
    }

    args[argnum] = NULL;
    return args;
}



/*
 * purpose: free the list returned by splitline
 * returns: nothing
 *  action: free all strings in list and then free the list
 * */
void freelist(char **list)
{
    char **cp = list;

    while (*cp)
    {
	free(*cp);
	++cp;
    }

    free(list);
}



void * emalloc(size_t n)
{
    void *rv;

    if ((rv = malloc(n)) == NULL)
	fatal("out of memory", "", 1);

    return rv;
}



void * erealloc(void *p, size_t n)
{
    void *rv;

    if ((rv = realloc(p, n)) == NULL)
	fatal("realloc() faild", "", 1);

    return rv;
}



/*
 * purpose: constructor for strings
 * returns: a string, never NULL
 * */
char * newstr(char *s, int l)
{
    char *rv = emalloc(l+1);

    rv[l] = '\0';
    strncpy(rv, s, l);

    return rv;
}


int up_arrow(int a, int b)
{
    /* update history index */
    if (history_index == -1)
        history_index = hist.last;
    do
    {
        history_index = (history_index-1 < 0) ?  MAX_HIST-1 : history_index-1;
    }
    while (hist.cmds[history_index] == NULL);

    if (first_time == 0)
        rl_do_undo();
    
    rl_insert_text(hist.cmds[history_index]);

    first_time = 0;
    return 0;
}


int down_arrow(int a, int b)
{
    /* update history index */
    do
    {
        history_index = (history_index+1 == MAX_HIST) ? 0 :  history_index+1;
    }
    while (hist.cmds[history_index] == NULL);

    if (first_time == 0)
        rl_do_undo();

    rl_insert_text(hist.cmds[history_index]);

    first_time = 0;
    return 0;
}

