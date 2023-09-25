#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

#define MSG "Hello"
#define BLK "     "

int row;
int col;
int dir;

int setTicker(int nMsec);
__sighandler_t moveMsg(int signum);

int main(int argc, char* argv[]){
    int delay;
    int ndelay;
    char c;

    initscr();
    crmode();
    noecho();
    clear();

    row = 10;
    col = 10;
    dir = 1;
    delay = 1000;

    move(row, col);
    addstr(MSG);
    signal(SIGALRM, moveMsg);
    setTicker(delay);

    while(1){
        ndelay = 0;
        c = getch();
        if(c == 'Q') break;
        if(c == ' ') dir = -dir;
        if(c == 'f' && delay > 2) ndelay = delay/2;
        if(c == 's') ndelay = delay*2;
        if(ndelay > 0) setTicker(delay = ndelay);
    }

    endwin();
    return 0;
}

int setTicker(int nMsec){
    struct itimerval newTimeset;
    long nSec, nUsec;
    
    nSec = nMsec / 1000;
    nUsec = (nMsec % 1000) * 1000L;

    newTimeset.it_interval.tv_sec = nSec;
    newTimeset.it_interval.tv_usec = nUsec;
    newTimeset.it_value.tv_sec = nSec;
    newTimeset.it_value.tv_usec = nUsec;

    return setitimer(ITIMER_REAL, &newTimeset, NULL);
}

__sighandler_t moveMsg(int signum){
    signal(SIGALRM, moveMsg);
    move(row, col);
    addstr(BLK);
    col += dir;
    move(row, col);
    addstr(MSG);
    refresh();
    if(dir == -1 && col <= 0) dir = 1;
    else if(dir == 1 && col+strlen(MSG) >= COLS) dir = -1;
}