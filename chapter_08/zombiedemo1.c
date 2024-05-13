/*
 * zombiedemo1.c - My program (not from the book)
 *     Illustrating a Zombie proccess.
 *     Run this program in your current terminal,
 *     and press "ps -fla" in another terminal.
 */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#define DELEY 10

int main()
{
    char *arglist[3];
    arglist[0] = "ls";
    arglist[1] = "-al";
    arglist[2] = NULL;

    switch (fork())
    {
	case -1:
	    perror("fork faild");
	    break;
        
	case 0:
	    execvp(arglist[0], arglist);
	    break;
	
	default:
	    sleep(DELEY); /* at this momment, 'ls' procces is a zombie */
	    break;
    }

    return 0;
}

