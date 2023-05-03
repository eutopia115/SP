#include <ncurses.h>
#include "Move/move.h"
#include "Struct/tree.h"
#include "Print/print_form.h"

#define ESCAPE 27
#define ENTER 10

char *fileItem[] = {"","move", "copy", "main menu", "credits"};

WINDOW **menuList(int start_col);
void makeMenubar(WINDOW *menubar);
void deleteMenu(WINDOW **items, int count);
int scrollMenu(WINDOW **items,int count,int menu_start_col, int index_num);
int checkQuit();
void initScreen();
void moveMode(WINDOW *scr);
void copyMode(WINDOW *scr);
void title(WINDOW *scr);
void credit(WINDOW *scr);
void moveOpt1(WINDOW *scr);
void moveOpt2(WINDOW *scr);
void moveOpt3(WINDOW *scr);
void moveOpt4(WINDOW *scr);
void moveOpt5(WINDOW *scr);
void copyOpt1(WINDOW *scr);
void copyOpt2(WINDOW *scr);
void copyOpt3(WINDOW *scr);
void copyOpt4(WINDOW *scr);
void copyOpt5(WINDOW *scr);

int main(){

    WINDOW *menubar, *exeScreen, *about;
    int key, status;
    initScreen();
    bkgd(COLOR_PAIR(1));
    menubar = subwin(stdscr, 1, 110, 0, 0);
    exeScreen = subwin(stdscr, 30, 110, 2, 1);
    title(exeScreen);
    makeMenubar(menubar);
    refresh();    
    while(1) {
        WINDOW **menuItems;            
        int selectedItem;

        key = getch();
        wrefresh(exeScreen);

        if (key == 'a' || key == 'A') {
            menuItems=menuList(0);
            selectedItem = scrollMenu(menuItems, 4, 20, 0); 
            deleteMenu(menuItems, 5);
            switch(selectedItem) {
                case 0 : moveMode(exeScreen); break;
                case 1 : copyMode(exeScreen); break;
                case 2 : title(exeScreen); break;
                case 3 : credit(exeScreen); break;
                default : wprintw(exeScreen, "Nothing have selected.");
            }
            touchwin(stdscr);
            refresh();    
        }
        if (key == ESCAPE || key == 'q' || key == 'Q') {
            if (checkQuit() == 1) break;
        }
        touchwin(stdscr);
        refresh();    
    }
    endwin();
}

WINDOW **menuList(int start_col){
    int i;
    WINDOW **items;
    items=(WINDOW **)malloc(5 * sizeof(WINDOW *)); 
    items[0]=newwin(6,19,1,start_col);
    wbkgd(items[0], COLOR_PAIR(2));
    box(items[0],ACS_VLINE, ACS_HLINE);
    items[1]=subwin(items[0], 1, 17, 2, start_col+1);
    items[2]=subwin(items[0], 1, 17, 3, start_col+1);
    items[3]=subwin(items[0], 1, 17, 4, start_col+1);
    items[4]=subwin(items[0], 1, 17, 5, start_col+1);
    for (i =1; i < 5; i++)
        wprintw(items[i], "%s",fileItem[i]);
    wbkgd(items[1], COLOR_PAIR(1));
    wrefresh(items[0]);
    return items;
}
int checkQuit(){
    WINDOW *check;
    int key;
    check = newwin(3, 40, 3, 5);
    wmove(check, 1, 2);
    wprintw(check, "Exit program (y/n) ? "); 
    wbkgd(check, COLOR_PAIR(2));
    box(check, ACS_VLINE, ACS_HLINE);
    refresh();
    key = wgetch(check);
    delwin(check);
    if (key == 'y' || key == 'Y') return 1;
    else return 0;
}
void deleteMenu(WINDOW **items, int count){
    int i;
    for (i = 0; i < count; i++)
        delwin(items[i]);
    free(items);
}
int scrollMenu(WINDOW **items,int count,int menu_start_col, int index_num){
    int key;
    int selected=0;
    while (1) {
        key=getch();
        if (key==KEY_DOWN || key==KEY_UP) {
            wbkgd(items[selected+1],COLOR_PAIR(2));
            wnoutrefresh(items[selected+1]);
            if (key==KEY_DOWN) selected=(selected+1) % count;
            else selected=(selected+count-1) % count;
            wbkgd(items[selected+1],COLOR_PAIR(1));
            wnoutrefresh(items[selected+1]);
            doupdate();
        } 
        else if (key==ESCAPE) return -1;
        else if (key==ENTER) return selected;
    }
}
void initScreen(){
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLUE, COLOR_WHITE);
    init_pair(3, COLOR_RED, COLOR_WHITE);
    curs_set(2);
    noecho();
    keypad(stdscr, TRUE);
}
void makeMenubar(WINDOW *menubar){
    wbkgd(menubar, COLOR_PAIR(2));
    wattron(menubar, COLOR_PAIR(3));
    waddstr(menubar, " Menu");
    wattron(menubar, COLOR_PAIR(3));
    waddstr(menubar, "(a)");
}

void moveMode(WINDOW *scr){
    int opt;
    werase(scr);
    mvwprintw(scr,1,2," Move Mode : Option Select Stage");
    mvwprintw(scr,2,2,"*****************************************************************************************");
    mvwprintw(scr,3,2," Press the button to choose option");  
    mvwprintw(scr,4,2," 1. Date Collection"); 
    mvwprintw(scr,5,2," 2. Extension Collection");     
    mvwprintw(scr,6,2," 3. Date Classify");   
    mvwprintw(scr,7,2," 4. Extension Filtering");   
    mvwprintw(scr,8,2," 5. Classify + Filtering");   
    mvwprintw(scr,9,2,"-----------------------------------------------------------------------------------------");   
    mvwprintw(scr,10,2," Option Detail");
    mvwprintw(scr,11,2," 1. collect files which modified time between timeX and timeY from home directory");   
    mvwprintw(scr,12,2," 2. collect files which extension is same with input from home directory");   
    mvwprintw(scr,13,2," 3. execute option 1 then classify by date from collection");   
    mvwprintw(scr,14,2," 4. execute option 1 then filter the extension by user input from collection");   
    mvwprintw(scr,15,2," 5. classify by date + filter by input = option 2 + 3");  
    mvwprintw(scr,16,2,"****************************************************************************************");   
    mvwprintw(scr,17,2," README");
    mvwprintw(scr,18,2," Move Mode can collect files in your disk based on modified time");   
    mvwprintw(scr,19,2," User will choose what to save and to delete among collected files");   
    mvwprintw(scr,20,2," This mode is made for finding and managing old file easily");   
    opt = wgetch(scr);
    switch (opt) {
        case '1' : moveOpt1(scr); break;
        case '2' : moveOpt2(scr); break;
        case '3' : moveOpt3(scr); break;
        case '4' : moveOpt4(scr); break;
        case '5' : moveOpt5(scr); break;
    }
}
void copyMode(WINDOW *scr){
    int opt;
    werase(scr);
    mvwprintw(scr,1,2,"Copy Mode : Option Select Stage");
    mvwprintw(scr,2,2,"*****************************************************************************************");
    mvwprintw(scr,3,2," Press the button to choose option");  
    mvwprintw(scr,4,2," 1. Date Collection"); 
    mvwprintw(scr,5,2," 2. Extension Collection");     
    mvwprintw(scr,6,2," 3. Date Classify");   
    mvwprintw(scr,7,2," 4. Extension Filtering");   
    mvwprintw(scr,8,2," 5. Classify + Filtering");   
    mvwprintw(scr,9,2,"-----------------------------------------------------------------------------------------");   
    mvwprintw(scr,10,2," Option Detail");
    mvwprintw(scr,11,2," 1. collect files which modified time between timeX and timeY from home directory");   
    mvwprintw(scr,12,2," 2. collect files which extension is same with input from home directory");   
    mvwprintw(scr,13,2," 3. execute option 1 then classify by date from collection");   
    mvwprintw(scr,14,2," 4. execute option 1 then filter the extension by user input from collection");   
    mvwprintw(scr,15,2," 5. classify by date + filter by input = option 2 + 3");  
    mvwprintw(scr,16,2,"****************************************************************************************");  
    mvwprintw(scr,17,2," README");
    mvwprintw(scr,18,2," Copy Mode can collect files in your disk based on modified time");   
    mvwprintw(scr,19,2," User will choose what to backup among collected files");   
    mvwprintw(scr,20,2," Backup space will be chosen by user's input");   
    mvwprintw(scr,21,2," This mode is made for backup old file easily");   
    opt = wgetch(scr);
    switch (opt) {
        case '1' : copyOpt1(scr); break;
        case '2' : copyOpt2(scr); break;
        case '3' : copyOpt3(scr); break;
        case '4' : copyOpt4(scr); break;
        case '5' : copyOpt5(scr); break;
    }
}

void title(WINDOW *scr){
    werase(scr);
    mvwprintw(scr,1,2,"----------------------------------");
    mvwprintw(scr,2,2,"-          File Manager          -");
    mvwprintw(scr,3,2,"----------------------------------");
    mvwprintw(scr,4,2,"-  Press the button              -");
    mvwprintw(scr,5,2,"-  a : open menu, q : quit       -");
    mvwprintw(scr,6,2,"----------------------------------");
}
void credit(WINDOW *scr){
    werase(scr);
    mvwprintw(scr,1,2,"  ELEC0462-002 System Programming");
    mvwprintw(scr,2,2,"  2023 Spring Semester Term Project");
    mvwprintw(scr,3,2,"**************************************");
    mvwprintw(scr,4,2,"  Team 6 : File manager");
    mvwprintw(scr,5,2,"**************************************");
    mvwprintw(scr,6,2,"           Members of Team 6        ");
    mvwprintw(scr,7,2,"      ID          Name        Major");
    mvwprintw(scr,8,2,"-------------------------------------");
    mvwprintw(scr,9,2,"  2018####32   Minkyu Kim      EE");
    mvwprintw(scr,10,2,"  2020####75   Sunwoo Ahn      CSE");
    mvwprintw(scr,11,2,"  2021115360   Donghyeok Seo   CSE");
    mvwprintw(scr,12,2,"**************************************");
    mvwprintw(scr,13,2,"  Version : 0.0, Date : 2023-04-30");
    mvwprintw(scr,14,2,"**************************************");
}

void moveOpt1(WINDOW *scr){
    int xYear, xMon, xDate, yYear, yMon, yDate;
    echo();
    werase(scr);
    mvwprintw(scr,1,2,"Option 1: Date Collection");
    mvwprintw(scr,2,2,"----------------------------------------------------------------");
    mvwprintw(scr,3,2," start time (distant past)");  
    mvwprintw(scr,4,2," Year : ");  
    mvwscanw(scr,4,12,"%d", &xYear); 
    mvwprintw(scr,5,2," Month : ");   
    mvwscanw(scr,5,12,"%d", &xMon);
    mvwprintw(scr,6,2," Date : ");   
    mvwscanw(scr,6,12,"%d", &xDate);
    mvwprintw(scr,7,2,"---------------------------------------------------------------");   
    mvwprintw(scr,8,2," end time (near past)"); 
    mvwprintw(scr,9,2," Year : ");  
    mvwscanw(scr,9,12,"%d",&yYear); 
    mvwprintw(scr,10,2," Month : ");   
    mvwscanw(scr,10,12,"%d", &yMon);
    mvwprintw(scr,11,2," Date : ");   
    mvwscanw(scr,11,12,"%d", &yDate);  
    mvwprintw(scr,12,2,"---------------------------------------------------------------");

}
void moveOpt2(WINDOW *scr){
    char* ext;
    echo();
    werase(scr);
    mvwprintw(scr,1,2,"Option 2 : Extension Collection");
    mvwprintw(scr,2,2,"----------------------------------------------------------------");
    mvwprintw(scr,3,2," String that name include (for instance : .c, .java)"); 
    mvwprintw(scr,4,2," String : ");  
    mvwscanw(scr,4,12,"%s", ext); 
    mvwprintw(scr,5,2,"---------------------------------------------------------------");
}
void moveOpt3(WINDOW *scr){
    int xYear, xMon, xDate, yYear, yMon, yDate;
    echo();
    werase(scr);
    mvwprintw(scr,1,2,"Option 3 : Date Classify");
    mvwprintw(scr,2,2,"----------------------------------------------------------------");
    mvwprintw(scr,3,2," start time (distant past)");  
    mvwprintw(scr,4,2," Year : ");  
    mvwscanw(scr,4,12,"%d", &xYear); 
    mvwprintw(scr,5,2," Month : ");   
    mvwscanw(scr,5,12,"%d", &xMon);
    mvwprintw(scr,6,2," Date : ");   
    mvwscanw(scr,6,12,"%d", &xDate);
    mvwprintw(scr,7,2,"---------------------------------------------------------------");   
    mvwprintw(scr,8,2," end time (near past)"); 
    mvwprintw(scr,9,2," Year : ");  
    mvwscanw(scr,9,12,"%d",&yYear); 
    mvwprintw(scr,10,2," Month : ");   
    mvwscanw(scr,10,12,"%d", &yMon);
    mvwprintw(scr,11,2," Date : ");   
    mvwscanw(scr,11,12,"%d", &yDate);  
    mvwprintw(scr,12,2,"---------------------------------------------------------------");
}
void moveOpt4(WINDOW *scr){
    int xYear, xMon, xDate, yYear, yMon, yDate, status;
    char* ext;
    echo();
    werase(scr);
    mvwprintw(scr,1,2,"Option 4 : Extension Filtering");
    mvwprintw(scr,2,2,"----------------------------------------------------------------");
    mvwprintw(scr,3,2," start time (distant past)");  
    mvwprintw(scr,4,2," Year : ");  
    mvwscanw(scr,4,12,"%d", &xYear); 
    mvwprintw(scr,5,2," Month : ");   
    mvwscanw(scr,5,12,"%d", &xMon);
    mvwprintw(scr,6,2," Date : ");   
    mvwscanw(scr,6,12,"%d", &xDate);
    mvwprintw(scr,7,2,"---------------------------------------------------------------");   
    mvwprintw(scr,8,2," end time (distant past)"); 
    mvwprintw(scr,9,2," Year : ");  
    mvwscanw(scr,9,12,"%d",&yYear); 
    mvwprintw(scr,10,2," Month : ");   
    mvwscanw(scr,10,12,"%d", &yMon);
    mvwprintw(scr,11,2," Date : ");   
    mvwscanw(scr,11,12,"%d", &yDate);  
    mvwprintw(scr,12,2,"---------------------------------------------------------------");
    mvwprintw(scr,13,2," String that name include (for instance : .c, .java, ID)"); 
    mvwprintw(scr,14,2," String : ");  
    mvwscanw(scr,14,12,"%s", ext); 
    mvwprintw(scr,15,2,"---------------------------------------------------------------");
}
void moveOpt5(WINDOW *scr){
    int xYear, xMon, xDate, yYear, yMon, yDate;
    char* str;
    echo();
    werase(scr);
    mvwprintw(scr,1,2,"Option 4 : Classify + Filtering");
    mvwprintw(scr,2,2,"----------------------------------------------------------------");
    mvwprintw(scr,3,2," start time (distant past)");  
    mvwprintw(scr,4,2," Year : ");  
    mvwscanw(scr,4,12,"%d", &xYear); 
    mvwprintw(scr,5,2," Month : ");   
    mvwscanw(scr,5,12,"%d", &xMon);
    mvwprintw(scr,6,2," Date : ");   
    mvwscanw(scr,6,12,"%d", &xDate);
    mvwprintw(scr,7,2,"---------------------------------------------------------------");   
    mvwprintw(scr,8,2," end time (distant past)"); 
    mvwprintw(scr,9,2," Year : ");  
    mvwscanw(scr,9,12,"%d",&yYear); 
    mvwprintw(scr,10,2," Month : ");   
    mvwscanw(scr,10,12,"%d", &yMon);
    mvwprintw(scr,11,2," Date : ");   
    mvwscanw(scr,11,12,"%d", &yDate);  
    mvwprintw(scr,12,2,"---------------------------------------------------------------");
    mvwprintw(scr,13,2," String that name include (for instance : .c, .java, ID)"); 
    mvwprintw(scr,14,2," String : ");  
    mvwscanw(scr,14,12,"%s", str); 
    mvwprintw(scr,15,2,"---------------------------------------------------------------");
}

void copyOpt1(WINDOW *scr){
    int xYear, xMon, xDate, yYear, yMon, yDate;
    echo();
    werase(scr);
    mvwprintw(scr,1,2,"Option 1 : Date Collection");
    mvwprintw(scr,2,2,"----------------------------------------------------------------");
    mvwprintw(scr,3,2," start time (distant past)");  
    mvwprintw(scr,4,2," Year : ");  
    mvwscanw(scr,4,12,"%d", &xYear); 
    mvwprintw(scr,5,2," Month : ");   
    mvwscanw(scr,5,12,"%d", &xMon);
    mvwprintw(scr,6,2," Date : ");   
    mvwscanw(scr,6,12,"%d", &xDate);
    mvwprintw(scr,7,2,"---------------------------------------------------------------");   
    mvwprintw(scr,8,2," end time (near past)"); 
    mvwprintw(scr,9,2," Year : ");  
    mvwscanw(scr,9,12,"%d",&yYear); 
    mvwprintw(scr,10,2," Month : ");   
    mvwscanw(scr,10,12,"%d", &yMon);
    mvwprintw(scr,11,2," Date : ");   
    mvwscanw(scr,11,12,"%d", &yDate);  
    mvwprintw(scr,12,2,"---------------------------------------------------------------");
}
void copyOpt2(WINDOW *scr){
    char* ext;
    echo();
    werase(scr);
    mvwprintw(scr,1,2,"Option 2 : Extension Collection");
    mvwprintw(scr,2,2,"----------------------------------------------------------------");
    mvwprintw(scr,3,2," String that name include (for instance : .c, .java)"); 
    mvwprintw(scr,4,2," String : ");  
    mvwscanw(scr,4,12,"%s", ext); 
    mvwprintw(scr,5,2,"---------------------------------------------------------------");
}
void copyOpt3(WINDOW *scr){
    int xYear, xMon, xDate, yYear, yMon, yDate;
    echo();
    werase(scr);
    mvwprintw(scr,1,2,"Option 3 : Date Classify");
    mvwprintw(scr,2,2,"----------------------------------------------------------------");
    mvwprintw(scr,3,2," start time (distant past)");  
    mvwprintw(scr,4,2," Year : ");  
    mvwscanw(scr,4,12,"%d", &xYear); 
    mvwprintw(scr,5,2," Month : ");   
    mvwscanw(scr,5,12,"%d", &xMon);
    mvwprintw(scr,6,2," Date : ");   
    mvwscanw(scr,6,12,"%d", &xDate);
    mvwprintw(scr,7,2,"---------------------------------------------------------------");   
    mvwprintw(scr,8,2," end time (near past)"); 
    mvwprintw(scr,9,2," Year : ");  
    mvwscanw(scr,9,12,"%d",&yYear); 
    mvwprintw(scr,10,2," Month : ");   
    mvwscanw(scr,10,12,"%d", &yMon);
    mvwprintw(scr,11,2," Date : ");   
    mvwscanw(scr,11,12,"%d", &yDate);  
    mvwprintw(scr,12,2,"---------------------------------------------------------------");
}
void copyOpt4(WINDOW *scr){
    int xYear, xMon, xDate, yYear, yMon, yDate;
    char* str;
    echo();
    werase(scr);
    mvwprintw(scr,1,2,"Option 4 : Extension Filtering");
    mvwprintw(scr,2,2,"----------------------------------------------------------------");
    mvwprintw(scr,3,2," start time (distant past)");  
    mvwprintw(scr,4,2," Year : ");  
    mvwscanw(scr,4,12,"%d", &xYear); 
    mvwprintw(scr,5,2," Month : ");   
    mvwscanw(scr,5,12,"%d", &xMon);
    mvwprintw(scr,6,2," Date : ");   
    mvwscanw(scr,6,12,"%d", &xDate);
    mvwprintw(scr,7,2,"---------------------------------------------------------------");   
    mvwprintw(scr,8,2," end time (distant past)"); 
    mvwprintw(scr,9,2," Year : ");  
    mvwscanw(scr,9,12,"%d",&yYear); 
    mvwprintw(scr,10,2," Month : ");   
    mvwscanw(scr,10,12,"%d", &yMon);
    mvwprintw(scr,11,2," Date : ");   
    mvwscanw(scr,11,12,"%d", &yDate);  
    mvwprintw(scr,12,2,"---------------------------------------------------------------");
    mvwprintw(scr,13,2," String that name include (for instance : .c, .java, ID)"); 
    mvwprintw(scr,14,2," String : ");  
    mvwscanw(scr,14,12,"%s", str); 
    mvwprintw(scr,15,2,"---------------------------------------------------------------");
}
void copyOpt5(WINDOW *scr){
    int xYear, xMon, xDate, yYear, yMon, yDate;
    char* str;
    echo();
    werase(scr);
    mvwprintw(scr,1,2,"Option 5 : Classify + Filtering");
    mvwprintw(scr,2,2,"----------------------------------------------------------------");
    mvwprintw(scr,3,2," start time (distant past)");  
    mvwprintw(scr,4,2," Year : ");  
    mvwscanw(scr,4,12,"%d", &xYear); 
    mvwprintw(scr,5,2," Month : ");   
    mvwscanw(scr,5,12,"%d", &xMon);
    mvwprintw(scr,6,2," Date : ");   
    mvwscanw(scr,6,12,"%d", &xDate);
    mvwprintw(scr,7,2,"---------------------------------------------------------------");   
    mvwprintw(scr,8,2," end time (distant past)"); 
    mvwprintw(scr,9,2," Year : ");  
    mvwscanw(scr,9,12,"%d",&yYear); 
    mvwprintw(scr,10,2," Month : ");   
    mvwscanw(scr,10,12,"%d", &yMon);
    mvwprintw(scr,11,2," Date : ");   
    mvwscanw(scr,11,12,"%d", &yDate);  
    mvwprintw(scr,12,2,"---------------------------------------------------------------");
    mvwprintw(scr,13,2," String that name include (for instance : .c, .java, ID)"); 
    mvwprintw(scr,14,2," String : ");  
    mvwscanw(scr,14,12,"%s", str); 
    mvwprintw(scr,15,2,"---------------------------------------------------------------");
}
