#include <stdio.h>
#include "smsh.h"

int isControlCommand(char*);
int doControlCommand(char**);
int okToExec();

int process(char** args){
    int rv = 0;
    if(args[0]==NULL) rv=0;
    else if(isControlCommand(args[0])) rv = doControlCommand(args);
    else if(okToExec()) rv = execute(args);
    return rv;
}