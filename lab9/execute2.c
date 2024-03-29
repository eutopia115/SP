#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "varlib.h"

int execute(char* argv[]){
    int pid;
    int childInfo = -1;
    extern char** environ;
    if(argv[0] == NULL) return 0;
    if((pid=fork())==-1) perror("fork");
    else if(pid == 0){
        environ = VLtable2environ();
        signal(SIGINT,SIG_DFL);
        signal(SIGQUIT,SIG_DFL);
        execvp(argv[0], argv);
        perror("Cannot execute command");
        exit(1);
    }
    else if (wait(&childInfo) == -1) perror("wait");
    return childInfo;
}
