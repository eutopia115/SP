#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 128

int main(int argc, char* argv[]){
    int sockfd, opResult, opCount;
    unsigned char buf[1024];
    struct sockaddr_in servaddr;

    if(argc<3){
        printf("usage : ./client  remotePort remoteAddress\n");
        return -1;
    }
    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("socket creation failed");
        return -1;
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(argv[2]);
    servaddr.sin_port=htons(atoi(argv[1]));

    if(connect(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr))){
        perror("connect error");
        return -1;
    }
    puts("connected...");
    printf("Operand Count : ");
    scanf("%d", &opCount);
    buf[0]=(unsigned char)opCount;
    if(opCount==0){
        write(sockfd,buf,4);
    }
    if(opCount!=buf[0]){
        int tmp[1]={opCount};
        buf[0]='x';
        write(sockfd,buf,1);
        write(sockfd,tmp,4);
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
    write(sockfd, buf, 1+(opCount)*4+(opCount-1));
    read(sockfd, &opResult, 4);
    if(opCount!=0) printf("Operation Result : %d\n", opResult);
    close(sockfd);
    return 0;
}