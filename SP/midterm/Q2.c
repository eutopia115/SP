#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <ctype.h>
#include <fcntl.h>

void calc();
double oper(int x, int y, char op);
void setNodelayMode();
char getResponse();

__sighandler_t charin();
__sighandler_t deno0();
__sighandler_t intign();

int main(){
    calc();
}  

void calc(){
    __pid_t pid;
    pid = getpid();
    signal(SIGINT,intign);
    signal(SIGUSR1, charin);
    signal(SIGFPE, deno0);
    while (1){
        double x,y;
        char op, ans;
        setDelayMode();
        printf("Enter the 1st number : ");
        if(scanf(" %lf",&x)==0) kill(pid,SIGUSR1);
        printf("Enter an operator(+, -, *, /) : ");
        scanf(" %c",&op);
        printf("Enter the 2nd number : ");
        if(scanf(" %lf",&y)==0) kill(pid,SIGUSR1);

        printf("%.2lf\n", oper(x, y, op));
        setNodelayMode();
        ans=getResponse();
    }
}

double oper(int x, int y, char op){
    switch (op) {
        case '+': return x+y;
        case '-': return x-y;
        case '*': return x*y;
        case '/': return x/y;
        default : printf("\nunvaild operator\n"); exit(1);
    }
}

void setNodelayMode(){
    int termflags=fcntl(0,F_GETFL);
    termflags |= O_NDELAY;
    fcntl(0,F_SETFL,termflags);
}

void setDelayMode(){
    int termflags=fcntl(0,F_GETFL);
    termflags &= ~O_NDELAY;
    fcntl(0,F_SETFL,termflags);
}

char getResponse(){
    int input;
    int time = 1;
    printf("would you like calculate again?(y/n) : ");
    fflush(stdout);
    while(1){
        sleep(5);
        input = tolower(getchar());
        if(input == 'y') return 'y';
        if(input == 'n') exit(0);
        //5초 지나면 y, n 둘다 작동함
        if(time-- == 0 ) {printf("\n5 second have passed\n");exit(1);}
    }
}


__sighandler_t charin(){
    printf("illegal number entered!\n");
    printf("SIGUSR1 signal accept! Exit program\n");
    exit(1);
}

__sighandler_t deno0(){
    printf("Denominator is 0!\n");
    printf("SIGFPE signal accept! Exit program\n");
    exit(1);
}

__sighandler_t intign(){
    printf("\nSIGINT signal received. IGNORING...\n!");
    signal(SIGINT,SIG_IGN);
}