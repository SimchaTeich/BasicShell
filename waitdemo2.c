/* waitdemo2.c - shows how parent gets child status
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define DELAY 5

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
    pid_t wait_rv;          /* return value from wait() */
    int child_status;
    int high_8, bit_7, low_7;

    wait_rv = wait(&child_status);
    printf("Done waiting for %d. wait() returned: %d\n", childpid, wait_rv);

    high_8 = child_status >> 8;     /* 1111 1111 0 000 0000 */
    bit_7  = child_status & 0x80;   /* 0000 0000 1 000 0000 */
    low_7  = child_status & 0x7f;   /* 0000 0000 0 111 1111 */
    printf("status: exit=%d, core=%d, sig=%d\n", high_8, bit_7, low_7);
}

