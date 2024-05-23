/* test about the copy of enviornment variables after fork()
 * */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

void child();
void parent();



int main()
{
    pid_t pid;
    char  *cp = getenv("CHANGE_ME");

    /* check for existance of environment variable CHANGE_ME=value1 */
    if (cp != NULL && strcmp(cp, "value1") == 0)
    {
        printf("Before fork(), CHANGE_ME=%s\n", cp);
    }
    else
    {
        printf("Please enter 'export CHANGE_ME=value1'\n");
	exit(1);
    }

    /* fork(), then in the child it sets CHANGE_ME=value2           */
    switch (pid = fork())
    {
    case 0:
        child();
    
    default:
	parent();
    }

    return 0;
}



/*
 * code for child
 * */
void child()
{
    char *cp = getenv("CHANGE_ME");
    char *cp2;

    printf("child: CHANGE_ME=%s\n", cp);
    printf("child: Change it...\n");
    
    sleep(1);

    cp2  = cp + strlen(cp) - 1;
    *cp2 = '2';
    printf("child: CHANGE_ME=%s\n", cp);
    printf("child: done\n");
    exit(0);
}



/* 
 * code for parent after fork()
 * */
void parent()
{
    while (wait(NULL) != -1)
	;

    printf("After wait(), CHANGE_ME=%s\n", getenv("CHANGE_ME"));
}

