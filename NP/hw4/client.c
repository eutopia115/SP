#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

void byDomain(char* domain);
void byIP(char* ip);
void halfClose(char* port, char* ip);

int main(int argc, char* argv[]){
    if(argc == 2) byDomain(argv[1]);
    else if (argc == 3) halfClose(argv[1], argv[2]);

    else {
        printf("usage : ./client <port> <IP> 'OR' ./client <domain>\n");
        exit(1);
    }
}

void byDomain(char* domain){
    struct hostent *host;
    struct sockaddr_in addr;
    host = gethostbyname(domain);
    if(host == NULL) { perror("gethostbyname() error"); exit(1); }
    printf("gethostbyname()\n");
    printf("Official name : %s \n", host->h_name);
    for(int i=0; host->h_aliases[i]; i++) printf("Aliases %d : %s \n", i, host->h_aliases[i]);
    printf("Address type : %s \n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
    for(int i=0; host->h_addr_list[i]; i++) 
        printf("IP addr %d : %s \n", i, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
    puts("");
    printf("gethostbyaddr()\n");
    for(int i=0; host->h_addr_list[i]; i++) byIP(inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
}

void byIP(char* ip){
        struct hostent *host;
        struct sockaddr_in addr;
        memset(&addr, BUFSIZ, sizeof(addr));
        addr.sin_addr.s_addr = inet_addr(ip);
        host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);
        if(host == NULL) { perror("gethostbyaddr() error"); exit(1); }
        printf("Official name : %s \n", host->h_name);
        for(int i=0; host->h_aliases[i]; i++) printf("Aliases %d : %s \n", i, host->h_aliases[i]);
        printf("Address type : %s \n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
        for(int i=0; host->h_addr_list[i]; i++) 
        printf("IP addr %d : %s \n", i, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
}

void halfClose(char* port, char* ip){
    int sockfd, opResult, readCnt;
    unsigned char buf[BUFSIZ];
    struct sockaddr_in servaddr;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0) { perror("socket creation failed"); exit(1); }
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(ip);
    servaddr.sin_port=htons(atoi(port));

    if(connect(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr))) { perror("connect error"); exit(1); }
    puts("Message from client");
    read(sockfd, buf, BUFSIZ);
    printf("%s\n",buf);
    close(sockfd);
}