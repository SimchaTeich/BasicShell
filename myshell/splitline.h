/* splitline.h
 *     public functions implemented inside splitline.c
 */



/*
 * purpose: read next command line from fp
 * returns: dynamiclly allocated string holding command line
 *  errors: NULL at EOF (not really an error)
 *          calls fatal from emalloc()
 *   notes: allocates space in BUFSIZ chunks.
 * */
char     * next_cmd(char *prompt, FILE *fp);



/*
 * purpose: split a line into array of white-space seperated tokens
 * returns: a NULL-terminated array of pointers to copies of the tokens
 *          or NULL if line if no tokens on the line
 *  action: traverse the array, locate strings, make copies
 *    note: strtok() could work, but we may want to add quotes later
 * */

char    ** splitline(char *line);



/*
 * purpose: free the list returned by splitline
 * returns: nothing
 *  action: free all strings in list and then free the list
 * */

void       freelist(char **list);



void     * emalloc(size_t n);
void     * erealloc(void *p, size_t n);



/*
 * purpose: constructor for strings
 * returns: a string, never NULL
 * */
char * newstr(char *s, int l);

