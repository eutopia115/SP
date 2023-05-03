#include <stdio.h>
#include "smsh.h"

int isControlCommand(char*);
int doControlCommand(char**);
int okToExec();
int isBuiltinCommand(char **);
int builtinCommand(char ** args, int* resultp);

int process(char** args){
    int rv = 0;
    int result;
    if(args[0]==NULL) rv=0;
    else if(isControlCommand(args[0])) rv = doControlCommand(args);
    else if(isBuiltinCommand(args)) rv = builtinCommand(args, &result);
    else if(okToExec()) rv = execute(args);
    return rv;
}