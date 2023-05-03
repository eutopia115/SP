#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void oddeven(int x);
int odd(int x);
int even(int x);

int i;

int main(){
    int num, childpid, status;
    printf("Enter the number of child processes : ");
    if(scanf("%d",&num)==0) {printf("the input type is not int\n"); exit(1);}
    for(i=0; i<num; i++) {
        childpid = fork();
        wait(&status);
        if(childpid == 0) {
            oddeven(getpid());           
            exit(0);
        }
    }
}

void oddeven(int x){
    switch(x%2){
        case 0 :  {printf("Child process %d [pid == %d] : result(even) = %d\n", i+1, x, even(x)); break;}
        case 1 :  {printf("Child process %d [pid == %d] : result(odd) = %d\n", i+1, x, odd(x)); break;}
    }
}

int odd(int x){
    int res = 1;
    while(x!=0){
        int t = x%10;
        if(t == 0) t = 1;
        res*=t;
        x/=10;
    }
    return res;
}

int even(int x){
    int res = 0;
    while(x!=0){
        int t = x%10;
        res+=t;
        x/=10;
    }
    return res;
}