/* controlflow.h
 *     public functions implemented inside controlflow.c
 */
enum states { NEUTRAL, WANT_THEN, THEN_BLOCK, ELSE_BLOCK };
enum results { SUCCESS, FAIL };

static int if_state  = NEUTRAL;
static int if_result = SUCCESS;
static int last_state = 0;



/*
 * purpose: determine the shell should execute a command
 * returns: 1 for yes, 0 for no
 * details: if in THEN_BLOCK and if_result was SUCCESS then yes
 *          if in THEN_BLOCK and if_result was FAIL    then no
 *          if in WANT_THEN  then syntax error (sh/shell is different)
 * */
int ok_to_execute();



/*
 * purpose: boolean to report if the command is
 *          a shell control command
 * results: 0 ro 1
 * */
int is_control_command(char *s);



/*
 * purpose: Process "if", "then", "fi" - change state or detect error
 * returns: 0 if ok, -1 for syntax error
 * */
int do_control_command(char **args);

