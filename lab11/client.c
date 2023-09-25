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
    char msg[HOSTLEN];
    int sockid, sockfd, msglen;

    sockid = socket(PF_INET, SOCK_STREAM, 0);
}