#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]){
    int sockfd, cSockfd, readCnt, enable =1;
    char buf[BUFSIZ];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    FILE *fp = fopen("text.txt", "r");

    if(argc<2) { printf("usage : ./server localPort\n"); exit(1); }
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0) { perror("socket creation failed"); exit(1); }
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(atoi(argv[1]));

    if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0) { perror("bind failed"); exit(1); }
    if(listen(sockfd, 5)<0) {perror("socket failed"); exit(1); }
    if((cSockfd=accept(sockfd,(struct sockaddr*)&cliaddr, &len))<0) { perror("accept error"); exit(1); }
    
    readCnt = fread((void*)buf, 1, BUFSIZ, fp);
    printf("%s\n", buf);
    write(cSockfd, buf, BUFSIZ);
    shutdown(cSockfd, SHUT_WR);
    fclose(fp);
    close(cSockfd);
    close(sockfd);
}