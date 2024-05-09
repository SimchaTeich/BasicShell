/* waitdemo1.c - shows how parent pauses until child finishes
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define DELAY 2

void child_code(int delay);
void parent_code(pid_t childpid);

main()
{
    pid_t newpid;

    printf("Before: mypid is %d\n", getpid());

    if ((newpid = fork()) == -1)
	perror("fork");
    else if (newpid == 0)
	child_code(DELAY);
    else
	parent_code(newpid);
}



/*
 * new proccess takes a nap and then exist
 */
void child_code(int delay)
{
    printf("child %d here. will sleep for %d seconds\n", getpid(), delay);
    sleep(delay);
    printf("child done. about to exit\n");
    exit(17);
}



/* 
 * parent waits for child then prints a messages
 */
void parent_code(pid_t childpid)
{
    pid_t wait_rv; /* return value from wait() */
    wait_rv = wait(NULL);
    printf("Done waiting for %d. wait() returned: %d\n", childpid, wait_rv);
}

