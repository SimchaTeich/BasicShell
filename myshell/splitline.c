#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myshell.h"    /* using fatal() */
#include "splitline.h"
#include "conio.h"

#define is_delim(x) ((x)==' ' || (x)=='\t')


int isarrow(char c);

static int history_index = -1;


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
    int  bufspace = 0;                    /* total size           */
    int  pos = 0;                         /* current position     */
    int  c;                               /* input char           */
    extern history hist;
    int  use_history = 0;                 /* 1 if using history    */
    history_index = -1;                   /* start from the begining evey time */


    printf("%s", prompt);                         /* prompt user  */
    while ((c = getch()) != EOF)
    {
	/* need some previous command ? */
	if(isarrow(c))
	{
	    use_history = 1;
	    
	    /* update the next index according to the arrow */
	    switch(getch())
	    {
            case 'A':
		if (history_index == -1)
		    history_index = hist.last;
		do
		{
		    history_index = (history_index-1 < 0) ?  MAX_HIST-1 : history_index-1;
		}
		while (hist.cmds[history_index] == NULL);
		break;
	    
	    case 'B':
		do
		{
		    history_index = (history_index+1 == MAX_HIST) ? 0 :  history_index+1;
		}
		while (hist.cmds[history_index] == NULL);
		break;
	    }

	    /* print the command to the string  */
            printf("\x1b[2K");    // Clear entire line
            printf("\x1b[1F\n");  // Move to beginning of previuos line
            printf("%s%s", prompt, hist.cmds[history_index]);

	    continue;
	}
	else if (!use_history)
	{
	    printf("%c", c);
	}

	if (use_history)
	{
	    /* return the command back to stdin */
            fflush(stdin);
	    ungetc('\n', stdin);
            for (int i = strlen(hist.cmds[history_index]) - 1; i >= 0; --i)
	    {
		ungetc(hist.cmds[history_index][i], stdin);
	    }

            printf("\x1b[2K");  // Clear entire line
            printf("\x1b[1F");  // Move to beginning of previuos line
	}

        /* need space? */
	if (pos + 1 >= bufspace)                  /* 1 for \0     */
	{
	    if (bufspace == 0)                    /* y: 1st time  */
		buf = emalloc(BUFSIZ);
	    else                                  /* or expand    */
		buf = erealloc(buf, bufspace+BUFSIZ);
	    
	    bufspace += BUFSIZ;                   /* update size  */
	}

	/* end of command? */
	if (c == '\n')
	    break;

	/* no, add to buffer */
	buf[pos] = c;
	++pos;
    }

    if (c == EOF && pos == 0)             /* EOF and no input     */
	return NULL;                      /* say so               */

    buf[pos] = '\0';
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
    char   spots    = 0;                  /* spots in table       */
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



int isarrow(char c)
{
    return (c == '\033' && getch() == '[');
}

