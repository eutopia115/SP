#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "smsh.h"

#define DFL_PROMPT "> "

void try(char *cmdline, int result);

int main(){
    char *cmdline, *cmdline2, *prompt;
    int result, process(char**), mem[BUFSIZ], i=0;
    void setup();
    char **arglist;
    prompt = DFL_PROMPT;
    setup();
    while ((cmdline=nextCmd(prompt, stdin))!=NULL){
        if(cmdline[strlen(cmdline)-1]=='&'){
            cmdline[strlen(cmdline)-1]='\0';
            if((arglist=splitline(cmdline))!=NULL){
                result = execute(arglist, 1);
                mem[i++]=result;
                freelist(arglist);
            }
            free(cmdline);
        }
        else if (strcmp(cmdline,"exit")==0){
            for(int j=0; j<i; j++) kill(mem[i], SIGINT);
            break;
        }
        else {
            if((arglist=splitline(cmdline))!=NULL){
                result = execute(arglist, 0);
                freelist(arglist);
                mem[i++]=result;
            }    
            free(cmdline);
        }
    }
    return 0;
}

void setup(){
    signal(SIGINT,SIG_IGN);
    signal(SIGQUIT,SIG_IGN);
}

void fatal(char* s1, char* s2, int n){
    fprintf(stderr, "Error : %s, %s\n", s1, s2);
    exit(n);
}
