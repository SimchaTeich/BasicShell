/* 
 * forkdemo1.c
 *     shows how fork create two proccesses, distinguishable
 *     by the different return values from fork()
 */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

main()
{
    pid_t ret_from_fork, mypid;

    mypid = getpid();                        /* who am i?      */
    printf("Before: my pid is %d\n", mypid); /* tell the world */

    ret_from_fork = fork();

    sleep(1);
    printf("After: my pid is %d, fork() said %d\n", getpid(), ret_from_fork);
}

