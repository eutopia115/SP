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
    if(argc != 3) { fprintf(stderr, "usage : pipe cmd1 cmd2"); exit(1); }
    if(pipe(pip)==-1) {oops("Cannot get a pipe,", 1);}
    if((pid=fork())==-1) {oops("Cannot fork", 2);}
    if(pid>0){//parent process
        close(pip[1]);
        if(dup2(pip[0],0)==-1) {oops("Cannot redirect stdin",3);} //read from '0' : stdin
        close(pip[0]);
        execlp(argv[2], argv[2], NULL);
        oops(argv[2], 4);
    }
    close(pip[0]);//child process
    if(dup2(pip[1],0)==-1) {oops("Cannot redirect stdout",4);} //write to '1' : stdout
    close(pip[1]);
    execlp(argv[1], argv[1], NULL);
    oops(argv[2], 5);
}

