#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXLINE 100

const char* TargetFile = "/etc/passwd";

int main(){
    int fd;
    char line[MAXLINE];
    fgets(line, MAXLINE, stdin); printf("%s", line);
    fgets(line, MAXLINE, stdin); printf("%s", line);
    fgets(line, MAXLINE, stdin); printf("%s", line);
    close(0);
    fd = open(TargetFile, O_RDONLY);
    if(fd != 0) {fprintf(stderr, "couldn't open data as fd 0\n"); exit(1);}
    fgets(line, MAXLINE, stdin); printf("%s", line);
    fgets(line, MAXLINE, stdin); printf("%s", line);
    fgets(line, MAXLINE, stdin); printf("%s", line);
}