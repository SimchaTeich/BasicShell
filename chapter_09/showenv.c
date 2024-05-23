/* showenv.c - shows how to read and print the environment.
 *             works just like the env command.
 */

#include <stdio.h>
#include <stdlib.h>

extern char **environ; /* points to the array of strings */

main()
{
    int i;

    for (i = 0; environ[i]; ++i)
	printf("%s\n", environ[i]);
}

