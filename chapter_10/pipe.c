/* pipe.c
 *     Demonstrates: how to create a pipeline from ine process to another
 *                   Takes two args, each a command and connects argv[1]'s
 *                   output to input of argv[2]
 *            Usage: pipe command1 command2
 *           Effect: command1 | command2
 *      Limitations: command do not take arguments
 *          Details: uses execlp() since known number of args
 *             Note: exchange child and parent and watch fun
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define oops(m,x) {perror(m); exit(x);}

int main(int argc, char *argv[])
{
    int      thepipe[2];    /* two file descriptors */
    int      newfd;         /* useful for pipes     */
    pid_t    pid;           /* and the pid          */

    /* check input valid */
    if (argc != 3)
    {
        fprintf(stderr, "usage: pipe cmd1 cmd2\n");
	exit(1);
    }

    /* get a pipe        */
    if (pipe(thepipe) == -1)
	oops("Cannot get a pipe", 1);

    
    /*************************************************/
    /* now we have a pipe, now let's get two process */
    /*************************************************/


    /* get a process     */
    if ((pid = fork()) == -1)
	oops("Cannot fork", 2);


    /*************************************************/
    /* right here, there are two process             */
    /* Parent will read from pipe.                   */
    /* Child will write into pipe                    */
    /*************************************************/

    /* Parent            */
    if (pid > 0)
    {
        close(thepipe[1]);            /* parent doesnt write to pipe */
        
	if(dup2(thepipe[0], 0) == -1)
            oops("could not redirect stdin", 3);
        
	close(thepipe[0]);            /* stdin is duped, close pipe  */
       
       execlp(argv[2], argv[2], NULL);
       oops(argv[2], 4);
    }

    /* Child             */
    close(thepipe[0]);                /* child doesnt read to pipe   */

    if (dup2(thepipe[1], 1) == -1)
	oops("could not redirect stdout", 5);

    close(thepipe[1]);                /* stdout is duped, close pipe */
    execlp(argv[1], argv[1], NULL);
    oops(argv[1], 6);
}

