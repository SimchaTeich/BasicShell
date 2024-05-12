/* Programming Exercises: 8.10
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define DELAY 5

void child_code(int delay);
void parent_code();
void sigchld_handler(int sig);

main()
{
    int i, n_children;
    pid_t pid;

    printf("Enter a number: ");
    scanf("%d", &n_children);
    getchar();                        /* clean the buffer */
    printf("Creating %d new proccesses...\n", n_children);

    signal(SIGCHLD, sigchld_handler);

    printf("Before: mypid is %d\n", getpid());

    for (i = 0; i < n_children; ++i)
    {
        if ((pid = fork()) == -1)
	{
	    while (wait(NULL) != -1)  /* dont forget the earlier fork()s */
	        ;
            fprintf(stderr, "fork i=%d\n", i);
	}
	else if(pid == 0)
	{
	    child_code(DELAY);
	}
    }

    parent_code();
}



/*
 * new proccess takes a nap and then exist
 */
void child_code(int delay)
{
    printf("child %d here. will sleep for %d seconds\n", getpid(), delay);
    sleep(delay);
    printf("child done. about to exit\n");
    exit(0);
}



/* 
 * parent printing until SIGCHLD comes
 */
void parent_code()
{
    while (1)
    {
        printf("Waiting..\n");
	sleep(1);
    }
}



/*
 * handler for sigchld try to collect all SIGCHLDs
 */
void sigchld_handler(int sig)
{
    pid_t wait_rv;
    int counter;
    
    counter = 0;
    while((wait_rv = wait(NULL)) != -1)
        printf("proccess number %d: wait() returned: %d\n", ++counter, wait_rv);

    exit(0);
}

