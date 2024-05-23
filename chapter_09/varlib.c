/* varlib.c
 *
 * a simple storage system to store name=value pairs
 * with facility to mark item as part of the environment
 *
 * interface:
 *     VLsotre(name, value)     retuens 1 for ok, 0 for no
 *     VLlookup(name)           returns stirng or NULL if not there
 *     VLlist()                 prints out current table
 *
 * environment-related funtions
 *     VLexport(name)           add name to list of env vars
 *     VLtable2environ()        copy from table to environ
 *     VLenviorn2table          copy from environ to table
 *
 * details:
 *      the table is tores as an array of structs that
 *      contins a flag for global and a single string of
 *      the form name=value.  This allows EZ addition to the
 *      environment.  It makes searching pretty easy, as
 *      long as you search for "name="
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "varlib.h"
#include <string.h>

#define MAXVARS 200             /* a linkes list would be nicer */

struct var
{
    char *str;                  /* name=val string */
    int  global;                /* a boolean       */
};

static struct var tab[MAXVARS]; /* the table */

/* private methods */
static char *new_string(char *name, char *val);
static struct var *find_item(char *name, int first_blank);


