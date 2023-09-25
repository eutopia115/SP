#include <stdio.h>
#include <string.h>
#include "smsh.h"

enum states {NEUTRAL, WANT_THEN, THEN_BLOCK};
enum results {SUCCESS, FAIL};
static int ifState = NEUTRAL;
static int ifResult = SUCCESS;
static int lastStat = 0;
int synErr(char*);

int okToExec(){
    int rv = 1;
    if(ifState==WANT_THEN){
        synErr("then Expected");
        rv=0;
    }
    else if (ifState==THEN_BLOCK && ifResult == SUCCESS) rv = 1;
    else if (ifState==THEN_BLOCK && ifResult == FAIL) rv = 0;
    return rv;
}

int isControlCommand(char* s){
    return (strcmp(s,"if")==0||strcmp(s,"then")==0||strcmp(s,"fi")==0) ;
}

int doControlCommand(char** args){
    char* cmd = args[0];
    int rv = -1;
    if(strcmp(cmd,"if") == 0){
        if(ifState != NEUTRAL) rv=synErr("if unexpected");
        else {
            lastStat = process(args+1);
            ifResult = (lastStat == 0 ? SUCCESS : FAIL);
            ifState = WANT_THEN;
            rv=0;
        }
    }
    else if (strcmp(cmd,"then")==0){
        if(ifState != WANT_THEN) rv=synErr("then unexpected");
        else{
            ifState=THEN_BLOCK;
            rv=0;
        }
    }
    else if (strcmp(cmd,"fi")==0){
        if (ifState != THEN_BLOCK) rv=synErr("fi unexpected");
        else {
            ifState = NEUTRAL;
            rv=0;
        }
    }
    else fatal("internal error processing", cmd, 2);
    return rv;
}

int synErr(char* s){
    ifState = NEUTRAL;
    fprintf(stderr, "syntax error : %s\n", s);
    return -1;
}