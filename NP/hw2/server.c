#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int operation(int x, int y, char op);

int main(int argc, char* argv[]){
    int sockfd, cSockfd, opCount, result=0, operand[128];
    unsigned char buf[1024], operator[127], opc;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;

    if(argc<2){
        printf("usage : ./server localPort\n");
        return -1;
    }
    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("socket creation failed");
        return -1;
    }

    int enable =1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(atoi(argv[1]));

    if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0){
        perror("bind failed");
        return -1;
    }

    if(listen(sockfd, 5)<0){
        perror("socket failed");
        return -1;
    }
    while(1){
    if((cSockfd=accept(sockfd,(struct sockaddr*)&cliaddr, &len))<0){
        perror("accept error");
        return -1;
    }
    read(cSockfd, &opc, 1);
    if(opc=='x') {
        read(cSockfd,&opCount,4);
        printf("Server Close(%d)\n", opCount);
        close(cSockfd);
        close(sockfd);
        exit(1);
    }
    opCount=(int)(opc);
    printf("Operand Count : %d\n", opCount);

    for(int i=0; i<opCount; i++){
        read(cSockfd, &operand[i],4);
        printf("Operand %d : %d\n", i, operand[i]);
    }

    for(int i=0; i<opCount-1; i++){
        read(cSockfd, &operator[i],1);
        printf("Operator %d : %c\n", i, operator[i]);
    }
    
    result=operand[0];
    for(int i=0; i<opCount-1; i++){
        result=operation(result, operand[i+1], operator[i]); 
    }
    printf("Operation result : %d\n",result);
    write(cSockfd, &result, 4);
    close(cSockfd);
    }
}

int operation(int x, int y, char op){
    switch (op){
        case '+': return x + y;
        case '-': return x - y;
        case '*': return x * y;
    }
}
