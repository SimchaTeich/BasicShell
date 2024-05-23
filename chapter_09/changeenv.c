/* changeenv.c - shows how to change the environment
 *               note: calls "env" to display its new setting
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

main()
{
    char *table[3];

    /* fill the table */
    table[0] = "TERM=vt100";
    table[1] = "HOME=/on/the/range";
    table[2] = 0;

    environ = table;                 /* point to the table */

    execlp("env", "env", NULL);      /* exec a program     */
}

