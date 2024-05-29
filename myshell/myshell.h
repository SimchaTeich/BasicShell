#define _MYSHELL_H_

#define YES           1
#define NO            0
#define PIPE_READ     0
#define PIPE_WRITE    1

typedef struct command
{
    char **arglist;          /* command after split     */
    int    pipe[2];          /* the right pipe          */
    int    pid;              /* pid of process run it   */
    int    result;           /* result from process     */

} command;



void    fatal(char *s1, char *s2, int n);

