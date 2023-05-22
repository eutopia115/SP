#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>

#define PORTNUM 13000
#define HOSTLEN 256
#define oops(msg) {perror(msg); exit(1);}

void sanitize(char* str);

int main(int argc, char* argv[]){
    struct sockaddr_in saddr;
    struct hostent *hp;
    char hostname[HOSTLEN], dirname[BUFSIZ-5], command[BUFSIZ];
    int csockfd, sockfd, dirlen, c;
    FILE* sockfpi, *sockfpo, *pipefp;

    if((sockfd = socket(PF_INET, SOCK_STREAM, 0))<0){
        perror("socket creation failed");
        return -1;
    }

    int enable =1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    saddr.sin_family = AF_INET;
    saddr.sin_port=htons(PORTNUM);
    if(bind(sockfd, (struct sockaddr*)&saddr, sizeof(saddr))<0){
        perror("bind failed");
        return -1;
    }

    if(listen(sockfd, 5)<0){
        perror("socket failed");
        return -1;
    }

    while(1){
        char* buf=(char*)malloc(BUFSIZ);
        if((csockfd = accept(sockfd, NULL, NULL))<0) oops("accept");
        printf("got a call\n");
        if((sockfpi = fdopen(csockfd, "r")) == NULL) oops("fdopen read");
        if(fgets(dirname, BUFSIZ-5, sockfpi) == NULL) oops("reading dirname");
        sanitize(dirname);
        if((sockfpo = fdopen(csockfd, "w")) == NULL) oops("fdopen write");
        sprintf(command, "ls %s", dirname);
        if((pipefp=popen(command,"r"))==NULL) oops("popen");
        fread(buf, 1, BUFSIZ, pipefp);
        write(csockfd,buf, BUFSIZ);
        free(buf);
        pclose(pipefp);
        fclose(sockfpi);
        fclose(sockfpo);
        close(csockfd);
    }   
}

void sanitize(char* str){
    char* src, * dest;
    for(src = dest = str; *src; src++)
        if(*src == '/' || isalnum(*src)) *dest++ = *src;
    *dest = '\0';
}