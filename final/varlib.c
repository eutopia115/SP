#include <stdio.h>
#include <stdlib.h>
#include "varlib.h"
#include <string.h>
#include <ctype.h>
#include <signal.h>

#define MAXVARS 200

typedef struct  {
    char *str;
    int global;
} var;

static var tab[MAXVARS];
static char *newString(char*, char*);
static var* findItem(char*, int);

int VLstore(char* name, char* val){
    var *itemp;
    char *s;
    int rv = 1;
    if(strlen(val)==0) {printf("cannot store!\n"); return rv;}
    if((itemp=findItem(name,1)) != NULL &&  (s=newString(name,val)) != NULL){
        if(itemp->str) free(itemp->str);
        itemp->str = s;
        rv = 0;
    }
    return rv;
}

int VLdelete(char* name){
    var* itemp;
    int rv = 1;
    if((itemp = findItem(name,0)) != NULL) {
        free(itemp->str);
        rv = 0;
    }
    else printf("there is no variable to delete\n");
    return rv;
}

char* newString(char* name, char* val){
    char* retval;
    retval = malloc(strlen(name)+strlen(val)+2);
    if(retval != NULL) sprintf(retval, "%s=%s", name, val);
    return retval;
}

char *VLlookup(char* name){
    var* itemp;
    if((itemp = findItem(name,0)) != NULL) return itemp->str+1+strlen(name);
    else return "There is no variable to read";
}

int VLexport(char* name){
    var* itemp;
    int rv = 1;
    if((itemp = findItem(name,0)) != NULL) {
        itemp->global = 1;
        rv = 0;
    }
    else if(VLstore(name,"")==1) rv=VLexport(name);
    return rv;
}

static var* findItem(char* name, int firstBlank){
    int i;
    int len = strlen(name);
    char *s;
    for(i=0; i<MAXVARS && tab[i].str != NULL; i++){
        s = tab[i].str;
        if(strncmp(s, name, len) == 0 && s[len] == '=') return &tab[i];
    }
    if(i<MAXVARS && firstBlank) return &tab[i];
    return NULL;
}

void VLlist(){
    int i;
    for(i=0; i<MAXVARS && tab[i].str != NULL; i++){
        if(tab[i].global) printf("   * %s\n",tab[i].str);
        else printf("    %s\n",tab[i].str);
    }
}

int VLenviron2table(char* env[]){
    int i;
    char* newString;
    for(i=0; env[i] != NULL ; i++){
        if(i == MAXVARS) return 0;
        newString = malloc(1+strlen(env[i]));
        if(newString==NULL) return 0;
        strcpy(newString, env[i]);
        tab[i].str = newString;
        tab[i].global = 1;
    }
    while (i<MAXVARS){
        tab[i].str = NULL;
        tab[i++].global = 0;
    }
    return 1;
}

char** VLtable2environ(){
    int i,j;
    int n=0;
    char** envtab;
    for(i=0; i<MAXVARS && tab[i].str != NULL; i++) 
        if(tab[i].global == 1) n++;
    envtab = (char**)malloc((n+1)*sizeof(char*));
    if(envtab == NULL) return NULL;
    for(i=0, j=0; i<MAXVARS && tab[i].str != NULL; i++){
        if(tab[i].global == 1) envtab[j++] = tab[i].str;
    }
    envtab[j] = NULL;
    return envtab;
}

int VLign(char* signame){
    for(int i=0; i<10; i++) signame[i]=toupper(signame[i]);
    if(strstr(signame, "HUP")) signal(1, SIG_IGN);
    else if(strstr(signame, "INT")) signal(2, SIG_IGN);
    else if(strstr(signame, "QUIT")) signal(3, SIG_IGN);
    else if(strstr(signame, "KILL")) signal(9, SIG_IGN);
    else if(strstr(signame, "VTALRM")) signal(26, SIG_IGN);
    else if(strstr(signame, "ILL")) signal(4, SIG_IGN);
    else if(strstr(signame, "TRAP")) signal(5, SIG_IGN);
    else if(strstr(signame, "IOT")) signal(6, SIG_IGN);
    else if(strstr(signame, "BUS")) signal(7, SIG_IGN);
    else if(strstr(signame, "FPE")) signal(8, SIG_IGN);
    else if(strstr(signame, "USR1")) signal(10, SIG_IGN);
    else if(strstr(signame, "SEGV")) signal(11, SIG_IGN);
    else if(strstr(signame, "USR2")) signal(12, SIG_IGN);
    else if(strstr(signame, "PIPE")) signal(13, SIG_IGN);
    else if(strstr(signame, "ALRM")) signal(14, SIG_IGN);
    else if(strstr(signame, "TERM")) signal(15, SIG_IGN);
    else if(strstr(signame, "TKFLT")) signal(16, SIG_IGN);
    else if(strstr(signame, "CHLD")) signal(17, SIG_IGN);
    else if(strstr(signame, "CONT")) signal(18, SIG_IGN);
    else if(strstr(signame, "STOP")) signal(19, SIG_IGN);
    else if(strstr(signame, "TSTP")) signal(20, SIG_IGN);
    else if(strstr(signame, "TTIN")) signal(21, SIG_IGN);
    else if(strstr(signame, "TTOU")) signal(22, SIG_IGN);
    else if(strstr(signame, "URG")) signal(23, SIG_IGN);
    else if(strstr(signame, "XCPU")) signal(24, SIG_IGN);
    else if(strstr(signame, "XFSZ")) signal(25, SIG_IGN);
    else if(strstr(signame, "PROF")) signal(27, SIG_IGN);
    else if(strstr(signame, "WINCH")) signal(28, SIG_IGN);
    else if(strstr(signame, "IO")) signal(29, SIG_IGN);
    else if(strstr(signame, "PWR")) signal(30, SIG_IGN);
    else if(strstr(signame, "UNUSED")) signal(31, SIG_IGN);
    else printf("Please enter appropriate signal name\n");
    return 0;
}
int VLdef(char* signame){
    for(int i=0; i<10; i++) signame[i]=toupper(signame[i]);
    if(strstr(signame, "HUP")) signal(1, SIG_DFL);
    else if(strstr(signame, "INT")) signal(2, SIG_DFL);
    else if(strstr(signame, "QUIT")) signal(3, SIG_DFL);
    else if(strstr(signame, "KILL")) signal(9, SIG_DFL);
    else if(strstr(signame, "VTALRM")) signal(26, SIG_DFL);
    else if(strstr(signame, "ILL")) signal(4, SIG_DFL);
    else if(strstr(signame, "TRAP")) signal(5, SIG_DFL);
    else if(strstr(signame, "IOT")) signal(6, SIG_DFL);
    else if(strstr(signame, "BUS")) signal(7, SIG_DFL);
    else if(strstr(signame, "FPE")) signal(8, SIG_DFL);
    else if(strstr(signame, "USR1")) signal(10, SIG_DFL);
    else if(strstr(signame, "SEGV")) signal(11, SIG_DFL);
    else if(strstr(signame, "USR2")) signal(12, SIG_DFL);
    else if(strstr(signame, "PIPE")) signal(13, SIG_DFL);
    else if(strstr(signame, "ALRM")) signal(14, SIG_DFL);
    else if(strstr(signame, "TERM")) signal(15, SIG_DFL);
    else if(strstr(signame, "TKFLT")) signal(16, SIG_DFL);
    else if(strstr(signame, "CHLD")) signal(17, SIG_DFL);
    else if(strstr(signame, "CONT")) signal(18, SIG_DFL);
    else if(strstr(signame, "STOP")) signal(19, SIG_DFL);
    else if(strstr(signame, "TSTP")) signal(20, SIG_DFL);
    else if(strstr(signame, "TTIN")) signal(21, SIG_DFL);
    else if(strstr(signame, "TTOU")) signal(22, SIG_DFL);
    else if(strstr(signame, "URG")) signal(23, SIG_DFL);
    else if(strstr(signame, "XCPU")) signal(24, SIG_DFL);
    else if(strstr(signame, "XFSZ")) signal(25, SIG_DFL);
    else if(strstr(signame, "PROF")) signal(27, SIG_DFL);
    else if(strstr(signame, "WINCH")) signal(28, SIG_DFL);
    else if(strstr(signame, "IO")) signal(29, SIG_DFL);
    else if(strstr(signame, "PWR")) signal(30, SIG_DFL);
    else if(strstr(signame, "UNUSED")) signal(31, SIG_DFL);
    else printf("Please enter appropriate signal name\n");
    return 0;
}
