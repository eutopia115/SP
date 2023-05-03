#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int sockfd, cSockfd, opCount, result, operand[255];
unsigned char buf[2048], operator[254], opc;
struct sockaddr_in servaddr, cliaddr;

int operation(int x, int y, char op);

int main(int argc, char* argv[]){
    int cliaddrSize = sizeof(cliaddr);

    if(argc<2){
        printf("usage : ./server localPort\n");
        return -1;
    }
    


    int enable =1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(atoi(argv[1]));

    while(1){
        if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0){
            perror("socket creation failed");
            return -1;  
        }
        if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0){
            perror("bind failed");
            return -1;
        }
        recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&cliaddr, &cliaddrSize);
        opc=buf[0];
        if(opc=='x') {
            opCount=0;
            opCount+=buf[1];
            opCount+=buf[2]*256;
            opCount+=buf[3]*256*256;
            opCount+=buf[4]*256*256*256;
            printf("Server Close(%d)\n", opCount);
            close(sockfd);
            exit(1);
        }
        opCount=(int)(opc);
        printf("Operand Count : %d\n", opCount);

        for(int i=0; i<opCount; i++){
            operand[i]+=buf[i*4+1];
            operand[i]+=buf[i*4+2]*256;
            operand[i]+=buf[i*4+3]*256*256;
            operand[i]+=buf[i*4+4]*256*256*256;
            printf("Operand %d : %d\n", i, operand[i]);
        }

        for(int i=0; i<opCount-1; i++){
            operator[i]=buf[1+opCount*4+i];
            printf("Operator %d : %c\n", i, operator[i]);
        }
        result=operand[0];
        for(int i=0; i<opCount-1; i++){
            result=operation(result, operand[i+1], operator[i]); 
        }
        printf("Operation result : %d\n",result);
        sendto(sockfd, &result, 4, 0, (struct sockaddr*)&cliaddr, cliaddrSize);
        close(sockfd);
    }
}

int operation(int x, int y, char op){
    switch (op){
        case '+': return x + y;
        case '-': return x - y;
        case '*': return x * y;
    }
}
