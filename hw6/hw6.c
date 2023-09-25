#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

char* symbol;
int startCol;
int startRow;
int topRow;
int botRow;
int lEdge;
int rEdge;
int rowDir;
int colDir;

int delay;
int ndelay;

int setTicker(int nMsec);
__sighandler_t moveMsg(int signum);

int main(int argc, char* argv[]){

    char c;
    signal(SIGQUIT,SIG_IGN);

    if(argc != 8) {
        printf("Usage : ./hw6 symbol startCol startRow topRow botRow lEdge rEdge\n"); 
        exit(1);
    }

    symbol = argv[1];
    startCol = atoi(argv[2]);
    startRow = atoi(argv[3]);
    topRow = atoi(argv[4]);
    botRow = atoi(argv[5]);
    lEdge = atoi(argv[6]);
    rEdge = atoi(argv[7]);

    initscr();
    crmode();
    noecho();
    clear();

    rowDir = 1;
    colDir = 1;
    delay = 250;

    move(topRow, lEdge);
    for(int i=0; i<=rEdge-lEdge; i++){
        addstr("*");
    }
    
    int tmp=atoi(argv[4]);
    for(int i=0; i<botRow-topRow-1; i++){
        move(++tmp, lEdge);
        addstr("*");
        move(tmp, rEdge);
        addstr("*");
    }

    move(botRow, lEdge);
    for(int i=0; i<=rEdge-lEdge; i++){
        addstr("*");
    }

    move(startRow, startCol);
    signal(SIGALRM, moveMsg);
    

    setTicker(delay);
    //축별 속도 4단까지 구현
    while(1){
        ndelay = 0;
        c = getch();
        if(c == 'Q') break;

        if(c == 'q' && abs(colDir) > 1) {
            if(colDir<0) colDir++;
            if(colDir>0) colDir--;
        }    
        if(c == 'w' && abs(rowDir) > 1) {
            if(rowDir<0) rowDir++;
            if(rowDir>0) rowDir--;
        }
        if(c == 'e' && abs(colDir) < 4) {
            if(colDir<0) colDir--;
            if(colDir>0) colDir++;
        }    
        if(c == 'r' && abs(rowDir) < 4) {
            if(rowDir<0) rowDir--;
            if(rowDir>0) rowDir++;
        }
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
    move(startRow, startCol); 
    addstr(" ");
    startCol += colDir;
    startRow += rowDir;
    move(startRow, startCol);
    addstr(symbol);
    refresh();
    if(startRow+rowDir<=topRow) {ndelay = delay*2; setTicker(delay = ndelay); move(startRow, startCol); addstr(" ");startRow=topRow+1; rowDir*=-1;}
    if(startCol+colDir<=lEdge) {ndelay = delay/2; setTicker(delay = ndelay); move(startRow, startCol); addstr(" ");startCol=lEdge+1; colDir*=-1;}
    if(startRow+rowDir>=botRow) {ndelay = delay*2; setTicker(delay = ndelay); move(startRow, startCol); addstr(" ");startRow=botRow-1; rowDir*=-1;}
    if(startCol+colDir>=rEdge) {ndelay = delay/2; setTicker(delay = ndelay); move(startRow, startCol); addstr(" ");startCol=rEdge-1; colDir*=-1;}
}
