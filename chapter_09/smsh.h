#define YES 1
#define NO  0

/* splitline.c */
char  * next_cmd();
char ** splitline(char * line);
void    freelist(char ** list);
void *  emalloc(size_t n);
void *  erealloc(void *p, size_t n);


/* executeX.c  */
int     execute(char ** arglist);


/* processX.c  */
int     process(char **args);


/* smshX.c     */
void    fatal(char *s1, char *s2, int n);

