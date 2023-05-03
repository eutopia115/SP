#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int execute(char* argv[], int option){
    int pid;
    int childInfo = -1;
    if(argv[0] == NULL) return 0;
    pid=fork();
    switch (option) {
    case 0:     
        if(pid == -1) perror("fork");
        else if(pid == 0){
            printf("Pid : %d\n", getpid());
            signal(SIGINT,SIG_DFL);
            signal(SIGQUIT,SIG_DFL);
            execvp(argv[0], argv);
            perror("Cannot execute command");
            exit(1);
        }
        else if (wait(&childInfo) == -1) perror("wait");
        return childInfo; 
    case 1:
        if(pid == -1) perror("fork");
        else if(pid == 0){
            printf("Pid : %d\n", getpid());
            signal(SIGINT,SIG_DFL);
            signal(SIGQUIT,SIG_DFL);
            execvp(argv[0], argv);
            perror("Cannot execute command");
            exit(1);
        }
        return childInfo; 
    }
    
}
