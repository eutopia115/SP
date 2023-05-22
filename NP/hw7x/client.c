#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/uio.h>

int main(int argc, char* argv[]){
    int send_sock, recv_sock, serv_sock, so_brd = 1, len;
    struct sockaddr_in broad_adr, recv_adr, serv_adr;
    char portNo[5], operand[BUFSIZ], buf[BUFSIZ];
    char msg[BUFSIZ] = "Where is Calc Server";
    int op[BUFSIZ], opCount, opResult;

    if(argc != 1){
        printf("Usage : ./client\n");
        exit(1);
    }

    send_sock = socket(PF_INET, SOCK_DGRAM, 0);

    memset(&broad_adr, 0, sizeof(broad_adr));
    broad_adr.sin_family = AF_INET;
    broad_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
    broad_adr.sin_port = htons(8080);
    
    setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));
    
    memset(msg, 0, BUFSIZ-1);
    printf("Start to find calc server.\n");
    sendto(send_sock, msg, strlen(msg), 0, (struct sockaddr*)&broad_adr, sizeof(broad_adr));
    
    memset(portNo, 0, BUFSIZ-1);
    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);

    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_adr.sin_port = htons(8082);
    if(bind(recv_sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr)) == -1){
        printf("bind error\n");
        exit(1);
    }
    memset(portNo, 0, BUFSIZ-1);
    len = recvfrom(recv_sock, portNo, 5, 0, NULL, 0);

    if(len < 0){
        printf("recvfrom error\n");
        exit(1);
    }
    if(strcmp(portNo,"fail")==0){
        printf("Fail\n");
        exit(1);
    }

    printf("Found calc server(%s)\n", portNo);

    if((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed\n");
        return -1;
    }

    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_adr.sin_port = htons(8081);

    if(connect(serv_sock, (const struct sockaddr *)&serv_adr, sizeof(serv_adr)) < 0){
        printf("connect error\n");
        exit(1);
    }
    memset(operand, 0, BUFSIZ-1);
    
    printf("Operand count: ");
    scanf("%d", &opCount);

    operand[0] = (unsigned char)opCount;
    if (opCount > 127 || opCount <= 0) {   
        close(serv_sock);
        exit(1);
    }
    for (int i = 0; i < opCount; i++) {
        printf("Operand %d: ", i);
        scanf("%d", (int*)&operand[1+4*i]);
    }
    for (int i = 0; i < opCount - 1; i++) {
        printf("Operator %d: ", i);
        scanf(" %c", &operand[1+opCount*4+i]);
    }
    len = write(serv_sock, operand, BUFSIZ);
    read(serv_sock, &opResult, sizeof(int));
    printf("Operation result: %d\n", opResult);
    close(serv_sock);
    return 0;
}
