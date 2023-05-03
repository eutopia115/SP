#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>

#define TRIES 3
#define SLEEPTIME 2
#define BEEP putchar('\a');
#define QUESTION "Do you want another transaction?"

int getOkChar();
int getResponse(char *q, int maxtries);
void ttyMode(int how);
void setCrNoechoMode();
void setNodelayMode();

int main(){
    int resp;
    ttyMode(0);
    setCrNoechoMode();
    setNodelayMode();
    resp=getResponse(QUESTION, TRIES);
    ttyMode(1);
    return resp;
}

int getOkChar(){
    int c;
    while((c=getchar())!=EOF && strchr("yYnN",c) == NULL);
    return c;
}

int getResponse(char *q, int maxtries){
    int input;
    printf("%s (y/n)?", q);
    fflush(stdout);
    while(1){
        sleep(SLEEPTIME);
        input = tolower(getOkChar());
        if(input == 'y') return 0;
        if(input == 'n') return 1;
        if(maxtries-- == 0 ) return 2;
        BEEP;
    }
}

void ttyMode(int how) {
    static struct termios originalMode;
    static int originalFlags;
    if(how == 0){
        tcgetattr(0, &originalMode);
        originalFlags=fcntl(0,F_GETFL);
    }
    else {
        tcsetattr(0,TCSANOW,&originalMode);
        fcntl(0,F_SETFL,originalFlags);
    }
}

void setCrNoechoMode(){
    struct termios ttystate;
    tcgetattr(0,&ttystate);
    ttystate.c_lflag &= ~ICANON;
    ttystate.c_lflag &= ~ECHO;
    ttystate.c_cc[VMIN]=1;
    tcsetattr(0,TCSANOW,&ttystate);
}

void setNodelayMode(){
    int termflags=fcntl(0,F_GETFL);
    termflags |= O_NDELAY;
    fcntl(0,F_SETFL,termflags);
}

