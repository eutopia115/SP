#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 128
unsigned char buf[1024];

int main(int argc, char* argv[]){
    int sockfd, opResult[1], opCount;
    struct sockaddr_in servaddr;
    int servaddrSize = sizeof(servaddr);

    if(argc<3){
        printf("usage : ./client remoteAddress remotePort\n");
        return -1;
    }
    
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0){
        perror("socket creation failed");
        return -1;
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(argv[1]);
    servaddr.sin_port=htons(atoi(argv[2]));

    printf("Operand Count : ");
    scanf("%d", &opCount);
    buf[0]=(unsigned char)opCount;
    if(opCount!=buf[0]){
        buf[0]='x';
        buf[1]+= opCount;
        buf[2]+= opCount >> 8 ;
        buf[3]+= opCount >> 16 ;
        buf[4]+= opCount >> 24 ;
        sendto(sockfd, buf, 5, 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
        close(sockfd);
        exit(1);
    }
    for(int i=0; i<opCount; i++){
        printf("Operand %d : ", i);
        scanf("%d",(int*)&buf[1+(i*4)]);
    }
    for(int i=0; i<opCount-1; i++){
        printf("Operator %d : ", i);
        scanf(" %c",&buf[(opCount*4+1)+i]);
    }
    sendto(sockfd, buf, 1+(opCount)*4+(opCount-1), 0, (struct sockaddr*)&servaddr, servaddrSize);
    int n = recvfrom(sockfd, opResult, 4, 0, (struct sockaddr*)&servaddr, &servaddrSize);
    close(sockfd);
    printf("Operation Result : %d\n", opResult[0]);
    return 0;
}