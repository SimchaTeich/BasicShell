/* test the heap after fork()
 * */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

void child(char *msg);
void parent(char *msg);



int main()
{
    pid_t pid;
    char  *msg = (char *)malloc(16 * sizeof(char));
    
    strcpy(msg, "Cyber is fun");
    printf("Before fork(): msg at %p\n", msg);
    printf("Before fork(): msg is %s\n", msg);

    /* fork(), then in the child it sets CHANGE_ME=value2           */
    switch (pid = fork())
    {
    case 0:
        child(msg);
    
    default:
	parent(msg);
    }

    return 0;
}



/*
 * code for child
 * */
void child(char *msg)
{
    printf("child: msg at %p\n", msg);
    printf("child: msg is %s\n", msg);
    
    printf("child: change it...\n");
    *(msg+3) = '3';                     /* from "Cyber" to "Cyb3r" */
    sleep(5);

    printf("child: msg at %p\n", msg);
    printf("child: msg is %s\n", msg);

    exit(0);
}



/* 
 * code for parent after fork()
 * */
void parent(char *msg)
{
    while (wait(NULL) != -1)
	;

    printf("After wait(), msg at %p\n", msg);
    printf("After wait(), msg is %s\n", msg);
}

