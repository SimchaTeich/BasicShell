#define _MYSHELL_H_

#define YES           1
#define NO            0
#define PIPE_READ     0
#define PIPE_WRITE    1

#define MAX_HIST      20     /* max commands history    */

typedef struct command
{
    char **arglist;          /* command after split     */
    int    pipe[2];          /* the right pipe          */
    int    pid;              /* pid of process run it   */
    int    result;           /* result from process     */

} command;



typedef struct history
{
    char *cmds[MAX_HIST];      /* this array with the 'last' index, implemented as a queue */
    int   last;                /* the index of the last command was enter.                 */
} history;


void    fatal(char *s1, char *s2, int n);
void    print_history();

