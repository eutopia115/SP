#define YES 1
#define NO 0

char *nextCmd();
char **splitline(char*);
void freelist(char**);
void *emalloc(size_t);
void *erealloc(void*, size_t);
int execute(char**, int);
void fatal(char*, char*, int);
int process(char**);
