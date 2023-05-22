#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

//final exam : ls | grep by C -> Must be test

void main(){
    int pid, fd;
    printf("About to run who into a file\n");
    if((pid=fork())== -1) {perror("fork"); exit(1);}
    if(pid == 0){
        close(1);
        fd = creat("userlist", 0644);
        printf("child process fd : %d\n", fd);
        execlp("who", "who", NULL);
        perror("execlp");
        exit(1);
    }
    else {
        wait(NULL);
        printf("Done Running \"who\", result in file \"userlist\"\n");
        printf("But, on my Windows Ubuntu \"who\" result is NONE\n");
    }
}