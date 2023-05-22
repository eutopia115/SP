#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define oops(m, x) {perror(m); exit(x);}

//pipe(int[2]) 1 : to write , 2 : to read;
//pipe system call : allocate 2 smallest file descriptor (big : write, small : read)

int main(int argc, char* argv[]){
    int pip[2], pid;
    if(pipe(pip)==-1) {oops("Cannot get a pipe,",1);}
    if((pid=fork())==-1) {oops("Cannot fork", 2);}
    if(pid>0){//parent process
        close(pip[1]);
        char str[BUFSIZ];
        printf("Enter a search String : ");
        scanf(" %s", str);  
        if(dup2(pip[0],0)==-1) {oops("Cannot redirect stdin",3);} //read from '0' : stdin
        close(pip[0]);
        execlp("grep", "grep", str, NULL);
        oops(argv[2], 0);
    }
    close(pip[0]);//child process
    if(dup2(pip[1],1)==-1) {oops("Cannot redirect stdout",4);} //write to '1' : stdout
    close(pip[1]);
    execlp("ls", "ls", NULL);
    oops(argv[1], 5);
}

