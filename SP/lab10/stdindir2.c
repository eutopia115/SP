#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXLINE 100
#define CLOSE_DUP // mode : open-close-dup-close 
//#define USE_DUP2 // mode : open-dup2-close

const char* TargetFile = "/etc/passwd";

int main(){
    int fd, newfd;
    char line[MAXLINE];
    fgets(line, MAXLINE, stdin); printf("%s", line);
    fgets(line, MAXLINE, stdin); printf("%s", line);
    fgets(line, MAXLINE, stdin); printf("%s", line);
    fd = open(TargetFile, O_RDONLY); // open first -> next file descriptor : smallest == 3
    #ifdef CLOSE_DUP
    close(0);
    newfd = dup(fd); // duplicate descriptor
    //printf("fd : %d\n", newfd);
    #else 
    newfd = dup2(fd,0); // duplicate descripor + allocate descriptor
    //printf("fd : %d\n", newfd);
    #endif
    //if(newfd != 0) {fprintf(stderr, "couldn't open data as fd 0\n"); exit(1);}
    close(fd);
    fgets(line, MAXLINE, stdin); printf("%s", line);
    fgets(line, MAXLINE, stdin); printf("%s", line);
    fgets(line, MAXLINE, stdin); printf("%s", line);
}