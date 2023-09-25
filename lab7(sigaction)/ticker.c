#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void cntDown(int signum);
int setTicker(int nMsec);

int main(int argc, char*argv[]){
    signal(SIGALRM, cntDown);
    if(setTicker(1000)==-1) perror("setTicker");
    else while(1) pause;
}

void cntDown(int signum){
    static int n = 5;
    printf("%d...", n--);
    fflush(stdout);
    if(n < 0) {printf("done!\n"); exit(0);}
}

int setTicker(int nMsec){
    struct  itimerval newTimeSet;
    long nSec, nUsec;
    nSec=nMsec/1000;
    nUsec=(nMsec%1000) * 1000L;
    newTimeSet.it_interval.tv_sec=nSec;
    newTimeSet.it_interval.tv_usec=nUsec;
    newTimeSet.it_value.tv_sec=nSec;
    newTimeSet.it_value.tv_usec=nUsec;
    return setitimer(ITIMER_REAL, &newTimeSet, NULL);
}