#define YES 1
#define NO  0

char *  next_cmd();
char ** splitline(char * line);
void    freelist(char ** list);
void *  emalloc(size_t n);
void *  erealloc(void *p, size_t n);
int     execute(char ** arglist);
void    fatal(char *s1, char *s2, int n);
int     process(char **args);
