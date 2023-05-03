#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


void hello();
void helloOption();

__sighandler_t xTotal();
__sighandler_t rTotal();
__sighandler_t xOption();
__sighandler_t rOption();

int i;
int n;

int main(int argc, char *argv[]){
    if(argc==1) hello();
    else if(strcmp(argv[1],"-n")==0){
        n=atoi(argv[2]);
        helloOption();
    }
    else printf("Usage : ./hw5 [-n positive int]\n");
}  

void hello(){
    while (1){
        sleep(1);
        switch (i%2)  {
            case 0: {
                signal(SIGINT, xTotal);
                printf("HelloWorld!\n");
                i++;
                break;
            }
       
            case 1: {
                signal(SIGINT, rTotal);
                printf("HelloWorld!\n");
                i++;
                break;
            }
        } 
    }
}

void helloOption(){
    for(int t=0; t<n; t++) {
        sleep(1);
        switch (i%2)  {
            case 0: {
                signal(SIGINT, xOption);
                printf("HelloWorld!\n");
                i++;
                break;
            }
       
            case 1: {
                signal(SIGINT, rOption);
                printf("HelloWorld!\n");
                i++;
                break;
            }
        } 
    }
    printf("=============================================\n");
    printf("Total number of printed HelloWorld! : %d\n", i);
    printf("Nunber of HelloWorld! prints remaining : %d\n", n-i);
    printf("=============================================\n");
}

__sighandler_t xTotal(){
    printf("\n=============================================\n");
    printf("Total number of printed HelloWorld! : %d\n", i);
    printf("=============================================\n");
    exit(1);
}

__sighandler_t rTotal(){
    printf("\n=============================================\n");
    printf("Total number of printed HelloWorld! : %d\n", i);
    printf("=============================================\n");
}


__sighandler_t xOption(){
    printf("\n=============================================\n");
    printf("Total number of printed HelloWorld! : %d\n", i);
    printf("Nunber of HelloWorld! prints remaining : %d\n", n-i);
    printf("=============================================\n");
    exit(1);
}

__sighandler_t rOption(){
    printf("\n=============================================\n");
    printf("Total number of printed HelloWorld! : %d\n", i);
    printf("Nunber of HelloWorld! prints remaining : %d\n", n-i);
    printf("=============================================\n");
}