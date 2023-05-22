#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

#define oops(m) {perror(m); exit(1);}
#define BSIZE 100

FILE *popen2(const char*, const char*);
void intign(int signo);
void quitign(int signo);
void killign(int signo);

int main(int argc, char* argv[]){
    FILE* pifp, *pofp;
    pid_t pid;
    char buf[BSIZE];
    struct sigaction intsig, quitsig, killsig;
    intsig.sa_handler = intign;
    sigemptyset(&intsig.sa_mask);
    sigaction(SIGINT, &intsig, NULL);
    quitsig.sa_handler = quitign;
    sigemptyset(&quitsig.sa_mask);
    sigaction(SIGQUIT, &quitsig, NULL);
    killsig.sa_handler = killign;
    sigemptyset(&killsig.sa_mask);
    sigaction(SIGTERM, &killsig, NULL);   
    pid = getpid();
    if((pifp=popen2("ls", "r"))== NULL) oops("popen2");
    if((pofp=popen2("sort -r", "w"))== NULL) oops("popen2");
    while (fgets(buf, BSIZE, pifp)) fputs(buf, pofp);
    pclose(pofp);
    pclose(pifp);
    sleep(1000);
    return 0;
}

FILE *popen2(const char* cmd, const char* mode){
    int pip[2];
    pid_t pid;
    FILE* ret;
    if(pipe(pip)==-1) {oops("Cannot get a pipe,");}
    if((pid=fork())==-1) {oops("Cannot fork");}
    if(strcmp(mode, "r")==0){
        if(pid>0){//parent process
            close(pip[1]);
            ret=fdopen(pip[0],"r");
            return ret;
        }
        close(pip[0]);//child process
        if(dup2(pip[1],1)==-1) {oops("Cannot redirect stdout");} //write to '1' : stdout
        close(pip[1]);
        execl("/bin/sh", "sh", "-c", cmd, NULL);
        exit(1);
    }
    if(strcmp(mode,"w")==0){
            if(pid>0){//parent process
            close(pip[0]);
            ret=fdopen(pip[1],"w");
            return ret;
        }
        pid=getpid();
        close(pip[1]);//child process
        if(dup2(pip[0],0)==-1) {oops("Cannot redirect stdout");} //write to '1' : stdout
        close(pip[0]);
        execl("/bin/sh", "sh", "-c", cmd, NULL);
    }
    kill(pid, SIGKILL);
    exit(1);
}

void intign(int signo){
    printf("\tSIGINT recieved but ignored\n");
}
void quitign(int signo){
    printf("\tSIGQUIT recieved but ignored\n");
}

void killign(int signo){
    printf("\tSIGKILL recieved but ignored\n");
}
