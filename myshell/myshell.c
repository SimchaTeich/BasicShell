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
void    free_pipeline();
void print_pipeline();


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
	print_pipeline();

	for (i = 0; i < pipeline_size; ++i)
	{
	     result = process(pipeline[i].arglist);
	}
	free(line);
	free_pipeline();
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
 * details: using strtok to divide the line.
 *          allocate command array to save
 *          the splited comands. if needed.
 *    note: line allways not NULL
 *  return: none. (command array is gloabl)
 * */
void init_pipeline(char *line)
{
    char    *line_to_split;
    char    *cmd;

    pipeline_size = 0;
    line_to_split = line;

    cmd = strtok(line_to_split, "|");
    while (cmd != NULL)
    {
	/* first time? */
	if (pipeline == NULL)
	{
	    pipeline = emalloc(sizeof(command));
            pipeline[pipeline_size].arglist = NULL;

            ++pipeline_capacity;
	}
	
	/* need more space? (add 1 each time because pipeline is not so long) */
	else if (pipeline_size == pipeline_capacity)
	{
	    pipeline = realloc(pipeline, (pipeline_size + 1) * sizeof(command));
	    pipeline[pipeline_size].arglist = NULL;
	    
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
}



/*********************/
/* helper functions  */
/*********************/


void print_pipeline()
{
    for (int i =0; i < pipeline_size; ++i)
    {
	printf("command[%d]: ", i);
	for (int j = 0; pipeline[i].arglist[j]; ++j)
	    printf("%s ", pipeline[i].arglist[j]);
	printf("\n");
    }

    printf("capacity: %d, size: %d\n", pipeline_capacity, pipeline_size);
}

