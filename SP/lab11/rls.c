#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORTNUM 13000
#define oops(msg) {perror(msg); exit(1);}

int main(int argc, char* argv[]){
    struct sockaddr_in saddr;
    struct hostent *hp;
    char buf[BUFSIZ];
    int sockid, sockfd, readlen;
    
    if(argc!=3) exit(-1);
    if((sockid = socket(PF_INET, SOCK_STREAM, 0))<0) oops("socket");
    bzero(&saddr, sizeof(saddr)); 
    if((hp = gethostbyname(argv[1]))==NULL) oops(argv[1]);
    bcopy(hp->h_addr, (struct sockaddr*)&saddr.sin_addr, hp->h_length);
    saddr.sin_family=AF_INET;
    saddr.sin_port = htons(PORTNUM);
    if(connect(sockid, (struct sockaddr*)&saddr, sizeof(saddr))!=0) oops("connect");
    if(write(sockid, argv[2], strlen(argv[2]))==-1) oops("write");
    if(write(sockid, "\n", 1) == -1) oops("write");
    readlen=read(sockid, buf, BUFSIZ);
    printf("%s\n",buf);
    close(sockid);
}