/* pipedemo.c
 *     Demonstrate: how to create and use pipe
 *          Effect: creates a pipe, writes into writing
 *                  end, then runs around and reads from 
 *                  reading end. A little weird, but
 *                  demonstrate the idea.
 */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int  len, i;
    int  apipe[2];       /* two file descriptors */
    char buf[BUFSIZ];    /* for reading end      */

    /* get a pipe */
    if (pipe(apipe) == -1)
    {
        perror("could not make a pipe");
	exit(1);
    }

    printf("Got a pipe! It is file descriptors: {%d %d}\n", apipe[0], apipe[1]);

    /* read from stdin, write into pipe, read from pipe, print */
    while (fgets(buf, BUFSIZ, stdin))
    {
        len = strlen(buf);
	if (write(apipe[1], buf, len) != len)    /* send down pipe  */
	{
	    perror("writing to pipe");
	    break;
	}

	for (i = 0; i < len; ++i)                /* wipe the buffer */
	    buf[i] = 'X';

	len = read(apipe[0], buf, BUFSIZ);       /* read from pipe  */
	if (len == -1)
	{
	    perror("reading from pipe");
	    break;
	}

	if (write(1, buf, len) != len)           /* send to stdout  */
	{
	    perror("writing to stdout");
	    break;
	}
    }

    return 0;
}

