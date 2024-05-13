/* 
 * forkdemo3.c - shows how return value from fork() allows a
 *               procces to determine whether it is a child
 *               or process
 */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

main()
{
    pid_t fork_rv;

    printf("Before: my pid is %d\n", getpid());
    fork_rv = fork();               /* create new proccess */
   
    if (fork_rv == -1)              /* check for error */
	perror("fork");
    else if (fork_rv == 0)
	printf("I am a child. my pid is %d\n", getpid());
    else
	printf("I am a parent. my child is %d\n", fork_rv);
}

