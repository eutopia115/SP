#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define MAXARGS 20
#define ARGLEN 100

void execute(char* arg[]);
char* makestring(char* buf);

int main(int argc, char* argv[]){
    char *arglist[MAXARGS+1];
    int numargs = 0;
    char argbuf[ARGLEN];

    while(numargs<MAXARGS){
        printf("ARG[%d]? : ", numargs);
        if(fgets(argbuf,ARGLEN,stdin) && *argbuf != '\n') arglist[numargs++] = makestring(argbuf);
        else if (numargs > 0){
            arglist[numargs] = NULL;
            execute(arglist);
            numargs = 0;
        }
    }
    return 0;
}

void execute(char* arg[]){
    int pid, exitstatus;
    pid = fork();
    switch (pid) {
    case -1 : {perror("fork failed"); exit(1);}
    case 0 : {execvp(arg[0], arg); perror("exevcp failed"); exit(1);}
    default: {
        while(wait(&exitstatus) != pid) 
            printf("child exited with status %d, %d\n", exitstatus>>8, exitstatus&0377);}
    }
}

char* makestring(char* buf){
    char *cp;
    buf[strlen(buf)-1] = '\0';
    cp = malloc(strlen(buf)+1);
    if(cp == NULL) {fprintf(stderr, "no memory"); exit(1);}
    strcpy(cp, buf);
    return cp;
}