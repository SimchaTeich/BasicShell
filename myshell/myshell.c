#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "myshell.h"
#include "splitline.h"
#include "process.h"
#include "varlib.h"



#define DFL_PROMPT "> "



/********************/
/* helper functions */
/********************/
void    setup();
void    init_pipeline(char *line);
//void    free_pipeline();
//void print_pipeline();


/* commands in pipeline are globals */
command * pipeline            = NULL;        /* contains the commands in pipeline      */
int       pipeline_capacity   = 0;           /* number of total room in pipeline       */
int       pipeline_size       = 0;           /* number of commands in current pipeline */
                                             /* Remember: size <= capacity             */
char    * prompt              = DFL_PROMPT;  /* contains the current prompt sign       */



int main()
{
    char    * line;       /* contains the current raw input       */
    char    * cmd;        /* a command input inside line          */
    int       i;          /* index to current command in pipeline */
    int       result;

    setup();
    

    while ((line = next_cmd(prompt, stdin)) != NULL)
    {
	init_pipeline(line);

	for (i = 0; i < pipeline_size; ++i)
	{
	    cmd = pipeline[i].cmd;
            if ((pipeline[i].arglist = splitline(cmd)) != NULL)
	    {
	         result = process(pipeline[i].arglist);
	         //freelist(arglist);
	    }
	}
	free(line);
    }

    return 0;
}



/*
 * purpose: initialize shell
 * returns: nothing. calls fatal() if trouble
 * */
void setup()
{
    extern char **environ;
    VLenviron2table(environ);
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}



void fatal(char *s1, char *s2, int n)
{
    fprintf(stderr, "Error: %s, %s\n", s1, s2);
    exit(n);
}



/*
 * purpose: break raw input to commands between pipe '|'
 * details: duplicate the line, and using strtok to
 *          divide the line. allocate command array
 *          to save the plited comands. if needed.
 *    note: line allways not NULL
 *  return: none. (command array is gloabl)
 * */
void init_pipeline(char *line)
{
    char    *line_to_split;
    char    *cmd;

    pipeline_size = 0;

    /* copy the line and work on the copy */
    line_to_split = newstr(line, strlen(line));

    cmd = strtok(line_to_split, "|");
    while (cmd != NULL)
    {
	/* first time?      */
	if (pipeline == NULL)
	{
	    pipeline = emalloc(sizeof(command));
	    ++pipeline_capacity;
	}

	/* need more space? (add 1 each time because pipeline is not so long) */
	else if (pipeline_size == pipeline_capacity)
	{
	    pipeline = realloc(pipeline, (pipeline_size + 1) * sizeof(command));
	    ++pipeline_capacity;
	}

	pipeline[pipeline_size].cmd = cmd;
	pipeline[pipeline_size].arglist = NULL;
	++pipeline_size;

	cmd = strtok(NULL, "|");
    }
}



/*********************/
/* helper functions  */
/*********************/


void print_pipeline()
{
    for (int i =0; i < pipeline_size; ++i)
	printf("command[%d]: %s\n" ,i, pipeline[i].cmd);

    printf("capacity: %d, size: %d\n", pipeline_capacity, pipeline_size);
}

