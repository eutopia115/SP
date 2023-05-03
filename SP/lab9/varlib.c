#include <stdio.h>
#include <stdlib.h>
#include "varlib.h"
#include <string.h>

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
    if((itemp=findItem(name,1)) != NULL &&  (s=newString(name,val)) != NULL){
        if(itemp->str) free(itemp->str);
        itemp->str = s;
        rv = 0;
    }
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