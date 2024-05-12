/* Programming Exercises: 8.8
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define DELAY 2
#define NUM_OF_CHILDREN 2

void child_code(int delay);
void parent_code(pid_t childpids[], int n_children);

main()
{
    int i;
    pid_t newpids[NUM_OF_CHILDREN];

    printf("Before: mypid is %d\n", getpid());

    for (i = 0; i < NUM_OF_CHILDREN; ++i)
    {
        if ((newpids[i] = fork()) == -1)
	{
	    while (wait(NULL) != -1)  /* dont forget the earlier fork()s */
	        ;
            fprintf(stderr, "fork i=%d\n", i);
	}
	else if(newpids[i] == 0)
	{
	    child_code(DELAY);
	}
    }

    parent_code(newpids, NUM_OF_CHILDREN);
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
 * parent waits for all childen
 */
void parent_code(pid_t childpids[], int n_children)
{
    pid_t wait_rv;
    
    while((wait_rv = wait(NULL)) != -1)
        printf("wait() returned: %d\n", wait_rv);
}

