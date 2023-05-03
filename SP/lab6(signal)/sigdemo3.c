#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

__sighandler_t elTime();
time_t start;

int main(){
    start=time(NULL);
    signal(SIGINT, elTime);
    printf("you cannot stop me\n");
    while (1){
        sleep(1);
        printf("haha\n");
    }
}  

__sighandler_t elTime(){
    time_t call=time(NULL);
    int el = call-start;
    printf("  Currently elapsed time : %d\n",el);
}