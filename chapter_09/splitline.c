/* splitline.c - command reading and parsing functions for smsh
 *
 *    char *next_cmd(char *prompt, FILE *fp) - get next command
 *    char **splitline(char *str);           - parse a string
 */

#include <stdio.h>
#include <stdlib.>
#include <string.h>
#include "smsh.h"

/*
 * purpose: read next command line from fp
 * returns: synamiclly allocated string holding command line
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

    printf("%s", prompt);                         /* prompt user  */
    while ((c = getc(fp)) != EOF)
    {
        /* need space? */
	if (pos + 1 >= bufspace)                  /* 1 for \0     */
	{
	    if (bufspace == 0)                    /* y: 1st time  */
		buf = emalloc(BUFSIZ);
	    else                                  /* or expand    */
		buf = erealloc(buf, bufspace+BUFSIZ)
	    
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

