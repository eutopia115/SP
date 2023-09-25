#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#define PORTNUM 13000
#define HOSTLEN 256
#define oops(msg) {perror(msg); exit(1);}

int main(int argc, char* argv[]){
    struct sockaddr_in saddr;
    struct hostent *hp;
    char hostname[HOSTLEN];
    int sockid, sockfd;
    FILE* sockfp;
    char* ctime();
    time_t thetime;
    if((sockid = socket(PF_INET, SOCK_STREAM, 0))<0) oops("socket");
    bzero((void*)&saddr, sizeof(saddr));
    gethostname(hostname, HOSTLEN);
    hp = gethostbyname(hostname);
    bcopy((void*)hp->h_addr, (void*)&saddr.sin_addr, hp->h_length);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORTNUM);
    if((bind(sockid, (struct sockaddr*)&saddr, sizeof(saddr)))!=0) oops("bind");
    if((listen(sockid,1))!=0) oops("listen");
    while(1){
        if((sockfd = accept(sockid, NULL, NULL))<0) oops("accept");
        printf("got a call\n");
        if((sockfp = fdopen(sockfd, "w"))<0) oops("fdopen");
        thetime = time(NULL);
        fprintf(sockfp, "the time here is ..");
        fprintf(sockfp, "%s", ctime(&thetime));
        fclose(sockfp);
    }
    
}