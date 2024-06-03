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



#define DFL_PROMPT "hello: "



/********************/
/* helper functions */
/********************/
void    setup();
void    init_pipeline(char *line);
void    free_pipeline();
void    sigint_handler(int sig);
void    init_history();
void    save_line(char *line);
void    free_history();
//void print_pipeline();

 

/* details of pipeline are globals      */
command * pipeline          = NULL;        /* contains the commands in pipeline      */
int       pipeline_capacity = 0;           /* number of total room in pipeline       */
int       pipeline_size     = 0;           /* number of commands in current pipeline */
                                           /* Remember: size <= capacity             */
int       cmdno             = 0;           /* current command index to run           */
                                           /* Remember: cmdno < size <= capasity     */


/* parameters for shell are globals too */
char    * prompt            = NULL;        /* contains the current prompt sign       */
int       last_result       = 0;           /* after each execute its get update      */
int       dont_wait         = 0;           /* 1 if '&' at the end of line. 0 for no  */
int       redirect_out      = 0;           /* 1 if '>' at the end of line. 0 for no  */
int       redirect_outa     = 0;           /* redirect out-append. sign is '>>'      */
int       redirect_in       = 0;           /* 1 if '<' at the end of line. 0 for no  */
int       redirect_err      = 0;           /* 1 if '2>' at the end of line. 0 for no */
char    * redirect_filename = NULL;        /* target filename to be directend        */
int       quit              = 0;           /* close the shell whene quit == 1        */


/* also history is global               */
history   hist;                            /* contains list of the last commands     */


int main()
{
    char *line;       /* contains the current raw input */

    setup();

    while ((line = next_cmd(prompt, stdin)) != NULL)
    {
	/* check if run the last command. (starts with '!!') */
	if (strlen(line) >= 2 && line[0] == '!' && line[1] == '!')
	{
	    if (hist.cmds[0] == NULL) /* if there is no history*/
	    {
	        free(line);
		line = NULL;
		continue;
	    }
	    printf("%s\n", hist.cmds[hist.last-1]);
            free(line);
	    line = newstr(hist.cmds[hist.last-1], strlen(hist.cmds[hist.last-1]));
	}

	/* save line for history */
        save_line(newstr(line, strlen(line)));
	
	init_pipeline(line);

	for (cmdno = 0; cmdno < pipeline_size; ++cmdno)
	{
            process();
	
	    if (quit)
	        break;
	}
	
	    /* clean before the next commmand */
	free(line);
	free_pipeline();
	dont_wait     = 0;
	redirect_out  = 0;
	redirect_outa = 0;
	redirect_in   = 0;
	redirect_err  = 0;
	if (redirect_filename != NULL)
	    free(redirect_filename);
	redirect_filename = NULL;

	if (quit)
	    break;
    }

    /* free the rest... */
    free_pipeline();
    free(prompt);
    free_history();

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

    init_history();

    prompt = newstr(DFL_PROMPT, strlen(DFL_PROMPT));

    signal(SIGINT, sigint_handler);
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



void sigint_handler(int sig)
{
    printf("You typed control-C!");
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



/*
 * purpose: init the struct history.
 * returns: nothing. using global
 * */
void init_history()
{
    int i;
    
    for (i = 0; i < MAX_HIST; ++i)
	hist.cmds[i] = NULL;

    hist.last = 0;
}



/*
 * purpose: append line to the history.
 * details: act like a cyclic queue.
 * */
void save_line(char *line)
{
    /* dont save an empty command... */
    if (line == NULL || line[0] == '\0')
	return;

    /* free the oldest command */
    if (hist.cmds[hist.last] != NULL)
	free(hist.cmds[hist.last]);

    hist.cmds[hist.last] = newstr(line, strlen(line));
    hist.last = (hist.last + 1) % MAX_HIST;
}



/*
 * purpose: prints the list of history commands
 * */
void print_history()
{
    int i       = hist.last;
    int count   = 1;
    int print_i = 0;

    while (count <= MAX_HIST)
    {
	if (hist.cmds[i] == NULL)
	{
	    i = (i + 1 == MAX_HIST) ? 0 : i + 1;
	    ++count;
	    continue;
	}

        ++print_i;
	printf("%4d %s\n", print_i, hist.cmds[i]);
	
	i = (i + 1 == MAX_HIST) ? 0 : i + 1;
	++count;
    }
}



/*
 * purpose: free history
 * */
void free_history()
{
    int i;
    for (i = 0; i < MAX_HIST; ++i)
    {
	if (hist.cmds[i] != NULL)
	{
            free(hist.cmds[i]);
	    hist.cmds[i] = NULL;
	}
    }

    hist.last = 0;
}

