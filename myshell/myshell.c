#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#ifndef _MYSHELL_H_
    #include "myshell.h"
#endif

#include "splitline.h"
#include "process.h"
#include "varlib.h"



#define DFL_PROMPT "> "



/********************/
/* helper functions */
/********************/
void    setup();
void    init_pipeline(char *line);
void    free_pipeline();
//void print_pipeline();
 

/* details of pipeline are globals */
command * pipeline            = NULL;        /* contains the commands in pipeline      */
int       pipeline_capacity   = 0;           /* number of total room in pipeline       */
int       pipeline_size       = 0;           /* number of commands in current pipeline */
                                             /* Remember: size <= capacity             */
int       cmdno               = 0;           /* current command index to run           */
                                             /* Remember: cmdno < size <= capasity     */
int       last_result         = 0;           /* after each execute its get update      */
char    * prompt              = DFL_PROMPT;  /* contains the current prompt sign       */



int main()
{
    char *line;       /* contains the current raw input */

    setup();

    while ((line = next_cmd(prompt, stdin)) != NULL)
    {
	init_pipeline(line);

	for (cmdno = 0; cmdno < pipeline_size; ++cmdno)
	{
	    process();
	}
	//print_pipeline();
	//printf("last_result: %d\n", last_result);
	free(line);
	free_pipeline();
    }

    free_pipeline();
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



/*********************/
/* helper functions  */
/*********************/



/*
 * purpose: break raw input to commands between pipe '|'
 * details: using strtok to divide the line.
 *          allocate command array to save
 *          the splited comands. if needed.
 *    note: line allways not NULL
 *  return: none. (command array is gloabl)
 * */
void init_pipeline(char *line)
{
    char    *cmd;

    pipeline_size = 0;

    cmd = strtok(line, "|");
    while (cmd != NULL)
    {
	/* first time? */
	if (pipeline == NULL)
	{
	    pipeline = emalloc(sizeof(command));
            pipeline[pipeline_size].arglist = NULL;
	    pipeline[pipeline_size].pid = -1;
            pipeline[pipeline_size].result = 0;

            ++pipeline_capacity;
	}
	
	/* need more space? (add 1 each time because pipeline is not so long) */
	else if (pipeline_size == pipeline_capacity)
	{
	    pipeline = realloc(pipeline, (pipeline_size + 1) * sizeof(command));
	    pipeline[pipeline_size].arglist = NULL;
	    pipeline[pipeline_size].pid = -1;
            pipeline[pipeline_size].result = 0;
	    
	    ++pipeline_capacity;
	}
	
        /* init the new arglist */
	if (pipeline[pipeline_size].arglist != NULL)
	    freelist(pipeline[pipeline_size].arglist);
	pipeline[pipeline_size].arglist = splitline(cmd);
	
	++pipeline_size;

	cmd = strtok(NULL, "|");
    }
}



/*
 * purpose: free all commands in the pipeline
 *          and the pipeline itself.
 * */
void free_pipeline()
{
    int i;

    for (i = 0; i < pipeline_capacity; ++i)
    {
	if (pipeline[i].arglist != NULL)
	{
            freelist(pipeline[i].arglist);
	    pipeline[i].arglist = NULL;
	}
    }

    free(pipeline);
    pipeline = NULL;

    pipeline_capacity = 0;
    pipeline_size     = 0;
    cmdno             = 0;
}



void print_pipeline()
{
    for (int i =0; i < pipeline_size; ++i)
    {
	printf("command[%d]: ", i);
	for (int j = 0; pipeline[i].arglist[j]; ++j)
	    printf("%s ", pipeline[i].arglist[j]);
	printf(" (pid: %d, result: %d)\n", pipeline[i].pid, pipeline[i].result);

    }

    printf("capacity: %d, size: %d\n", pipeline_capacity, pipeline_size);
}

