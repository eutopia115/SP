#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include "smsh.h"
#include "varlib.h"

int assign(char * );
int okname(char * );

int builtinCommand(char ** args, int* resultp){
    int rv=0;
    if(strcmp(args[0],"set")==0){
        VLlist();
        *resultp = 0;
        rv=1;
    }
    else if (strchr(args[0],'=')!=NULL){
        *resultp = assign(args[0]);
        if(*resultp != -1) rv = 1;
    }
    else if (strcmp(args[0],"export")==0){
        if(args[1] != NULL && okname(args[1])) *resultp = VLexport(args[1]);
        else *resultp = 1;
        rv = 1;
    }
    else if (strcmp(args[0],"exit")==0){
        exit(0);
    }
    else if (strcmp(args[0],"unset")==0){
        *resultp = VLdelete(args[1]);
        if(*resultp != -1) rv = 1;
    }
    else if (strcmp(args[0],"echo")==0){
        char tmp[BUFSIZ], tmp2[BUFSIZ];
        if(strchr(args[1], '$')) {
            strcpy(tmp, VLlookup(args[1]+1));
            printf("%s\n", tmp);
        }
        else printf("%s\n", args[1]);
    }
    else if (strcmp(args[0],"sig")==0){ // signalhandle ignore, default
    // > sig (- : ignore, + : default)(signal name) 
    // ex) sig -sigterm  -> ignore SIGTERM signal
    // upper letter, lower letter, short term : ex)sig -int -> available 
        if(args[1] == NULL) {printf("Usage : sig (- : ignore, + : default)(signal name)\n"); return -1;}
        if(strchr(args[1], '-')) VLign(args[1]+1); // ignore signal    
        else if(strchr(args[1], '+')) VLdef(args[1]+1); // default function of signal
        else printf("Usage : sig (- : ignore, + : default)(signal name)\n");
    }
    return rv;
}

int isBuiltinCommand(char ** args){
    return (strcmp(args[0],"set")==0||strchr(args[0],'=')!=NULL
    ||strcmp(args[0],"export")==0||strcmp(args[0],"exit")==0
    ||strcmp(args[0],"unset")==0 ||strcmp(args[0],"echo")==0
    ||strcmp(args[0],"sig")==0);
}

int assign(char* str){
    char* cp;
    int rv;
    cp = strchr(str,'=');
    *cp = '\0';
    rv = (okname(str) ? VLstore(str,cp+1) : -1);
    *cp = '=';
    return rv;
}

int okname(char* str){
    char* cp;
    for(cp = str;*cp;cp++)
        if ((isdigit(*cp) && cp == str) || !(isalnum(*cp) || *cp=='_')) return 0;
    return (cp != str);
}
