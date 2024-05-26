/* whotofile.c
 *      purpose: show how to redirect output for another program
 *         idea: fork, then in the child, redirect output, then exec
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    int   fd;

    printf("About to run who into a file\n");

    /* create a new process or quit */
    if ((pid = fork()) == -1)
    {
	perror("fork");
	exit(1);
    }

    /* child does the work          */
    if (pid == 0)
    {
        close(1);                        /* close,    */
	fd = creat("userlist", 0644);    /* then open */
	execlp("who", "who", NULL);      /* and run   */
	perror("execlp");
	
	exit(1);
    }

    /* parent wait then report      */
    if (pid != 0)
    {
        wait(NULL);
	printf("Done running who. results in userlist\n");
    }

    return 0;
}

