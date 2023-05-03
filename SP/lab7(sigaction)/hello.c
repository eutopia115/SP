#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <signal.h>

#define LEDGE 10
#define REDGE 30
#define ROW 10

int main(int argc, char* argv[]){
    char msg[] = "hello";
    char blank[] = "      ";
    int dir = 1;
    int pos = LEDGE;
    initscr();
    clear();
    while (1) {
        move(ROW,pos);
        addstr(msg);
        move(LINES-1,COLS-1);
        refresh();
        usleep(1500000);
        move(ROW,pos);
        addstr(blank);
        pos += dir;
        if(pos >= REDGE) dir -= 1;
        if(pos <= LEDGE) dir += 1;
    }
}