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
        fd = creat("pslist", 0644);
        execlp("ps", "ps", "aux", NULL);
        perror("execlp");
        exit(1);
    }
    else {
        wait(NULL);
        printf("Done Running \"ps\", result in file \"pslist\"\n");
    }
}