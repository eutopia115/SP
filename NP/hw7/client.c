#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/uio.h>

#define BUF_SIZE 100

void error_handling(char *message);

int main(int argc, char** argv){

    /* Variable that sending BroadCasting */
    int send_sock;
    struct sockaddr_in broad_adr;
    char sendmsg[] = "Where is Calc Server";
    char buf[BUF_SIZE];
    int so_brd = 1;

    /* Variable that receiving Port Number */
    int recv_sock;
    int port_len;
    struct sockaddr_in recv_adr;
    char PORT_ID[5];

    /* Multicasting Varaible */
    int sockfd, i;
    int str_len;
    struct sockaddr_in serv_adr;
    int write_len;

    /* Input Variable to calculating */
    char Operand[BUF_SIZE];
    int opCount;
    int op[BUF_SIZE];
    int opResult;

    if(argc != 1){
        printf("Usage : ./%s\n", argv[0]);
        exit(1);
    }

    /* Setting socket to sending request */
    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&broad_adr, 0, sizeof(broad_adr));
    broad_adr.sin_family = AF_INET;
    broad_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
    broad_adr.sin_port = htons(atoi("8080"));

    setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));

    /* Sending Message to find port number */
    memset(sendmsg, 0, BUF_SIZE-1);
    fprintf(stdout, "Start to find calc server.\n");
    sendto(send_sock, sendmsg, strlen(sendmsg), 0, (struct sockaddr*)&broad_adr, sizeof(broad_adr));

    /* Receiving Message that port number */
    memset(PORT_ID, 0, BUF_SIZE-1);

    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_adr.sin_port = htons(atoi("8082"));

    if(bind(recv_sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr)) == -1)
        perror("bind()");

    memset(PORT_ID, 0, BUF_SIZE-1);
    port_len = recvfrom(recv_sock, PORT_ID, 5, 0, NULL, 0);

    if(port_len < 0){
        fprintf(stderr,"recvfrom() error");
        exit(1);
    }
    if(!strcmp(PORT_ID,"NO")){
        fprintf(stdout, "Fail\n");
        return 0;
    }

    printf("Found calc server(%s)\n", PORT_ID);

    /* Starting Calculating */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket creation failed\n");
        return -1;
    }

    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_adr.sin_port = htons(atoi("8081"));

    if(connect(sockfd, (const struct sockaddr *)&serv_adr, sizeof(serv_adr)) < 0){
        perror("connect error");
        return -1;
    }
    memset(Operand, 0, BUF_SIZE-1);
    
    printf("Operand count: ");
    scanf("%d", &opCount);

    // break point
    Operand[0] = (unsigned char)opCount;
    if (opCount > 127 || opCount <= 0)
    {   
        fprintf(stdout, "Operand count: %d\n",opCount);
        close(sockfd);
        return 0;
    }
    for (int i = 0; i < opCount; i++)
    {
        printf("Operand %d: ", i);
        scanf("%d", (int *)&Operand[1 + 4 * i]);
    }
    for (int i = 0; i < opCount - 1; i++)
    {
        printf("Operator %d: ", i);
        scanf(" %c", &Operand[1 + opCount * 4 + i]);
    }
    write_len = write(sockfd, Operand, BUF_SIZE);
    read(sockfd, &opResult, sizeof(int));
    printf("Operation result: %d\n", opResult);
    close(sockfd);
    return 0;
}
