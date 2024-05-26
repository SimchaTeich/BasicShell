/* listargs.c
 *              print the number of command line args, list the args,
 *              then print a message to stderr
 */
#include <stdio.h>

main(int argc, char *argv[])
{
    int i;

    printf("Number of args: %d, Args are:\n", argc);

    for (i = 0; i < argc; ++i)
        printf("args[%d] %s\n", i, argv[i]);

    fprintf(stderr, "This message is sent to stderr.\n");

    return 0;
}

