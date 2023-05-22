#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <errno.h>

#define BUF_SIZE 100
#define ON 1
#define OFF 0

/* OPERATING Variable for second server found */
int OPERATING = OFF;

void error_handling(char *message);

// Exit handler
void read_childproc(int sig)
{
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status))
    {
        printf("Removed proc id: %d\n", pid);
    }
}

/* Main Functions */
void discovery_server();
char* arr_slicing(char* arr, int start, int end);
void calculator_server();

int main(int argc, char** argv){
    int cal_sock;
    int dis_sock;
    struct sockaddr_in broad_adr;
    char buf[BUF_SIZE];
    int so_brd = 1;

    if(argc != 2){
        printf("usage : %s <MENU>\n",argv[0]);
        exit(1);
    }

    if(!strcmp(argv[1],"discovery")){
        discovery_server();
    }

    if(!strcmp(argv[1], "calc")){
        calculator_server();
    }

}

void discovery_server(){
    /* Varaibles that receiving server port */
    int dis_sock, str_len;
    struct sockaddr_in broad_adr;
    char recv_data[BUF_SIZE];
    char *string_check = "server:";
    char *judge;
    char *CAL_ID;
    int so_brd = 1;

    /* Variables to send ok or fail. */
    int catch_sock;
    struct sockaddr_in send_adr;

    /* Variables to send Port Number to client. */
    int cli_sock;
    struct sockaddr_in clnt_adr;
    char NO_PORT[] = "NO";

    /* Receiving Server code */
    dis_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&broad_adr, 0, sizeof(broad_adr));
    broad_adr.sin_family = AF_INET;
    broad_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    broad_adr.sin_port = htons(atoi("8080"));
    
    fprintf(stdout, "Discovery Server operating...\n");
    
    if(bind(dis_sock, (struct sockaddr*)&broad_adr, sizeof(broad_adr)) == -1)
        perror("bind()");
    
    while(1){
        memset(recv_data, 0, sizeof(recv_data));
        str_len = recvfrom(dis_sock, recv_data, BUF_SIZE-1, 0, NULL, 0);
        if(str_len < 0)
            break;
        judge = arr_slicing(recv_data,0,7);
        if(!strcmp(judge, string_check)){
            /* Sending server code */
            catch_sock = socket(PF_INET, SOCK_DGRAM, 0);
            memset(&send_adr, 0, sizeof(send_adr));
            send_adr.sin_family = AF_INET;
            send_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
            send_adr.sin_port = htons(atoi("8081"));
            setsockopt(catch_sock, SOL_SOCKET, SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));

            if(OPERATING){
                sendto(catch_sock, "fail", strlen("fail"), 0, (struct sockaddr*)&broad_adr, sizeof(broad_adr));
                continue;
            }
            OPERATING = ON;
            CAL_ID = arr_slicing(recv_data, 7, 12);

            sendto(catch_sock, CAL_ID, strlen(CAL_ID), 0, (struct sockaddr*)&send_adr, sizeof(send_adr));
            fprintf(stdout, "Calc Server(%s) registerd\n", CAL_ID);
            close(catch_sock);
        }else{
            cli_sock = socket(PF_INET, SOCK_DGRAM, 0);
            memset(&send_adr, 0, sizeof(send_adr));
            clnt_adr.sin_family = AF_INET;
            clnt_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
            clnt_adr.sin_port = htons(atoi("8082"));
            setsockopt(catch_sock, SOL_SOCKET, SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));
            if(OPERATING)
                sendto(cli_sock, CAL_ID, strlen(CAL_ID), 0, (struct sockaddr*)&clnt_adr, sizeof(clnt_adr));
            else
                sendto(cli_sock, NO_PORT, strlen(NO_PORT), 0, (struct sockaddr*)&clnt_adr, sizeof(clnt_adr));
            close(cli_sock);
        }
    }
    close(dis_sock);
}

char* arr_slicing(char* arr, int start, int end){
    int arr_size = end - start;
    char* slice_arr  = (char*)malloc(sizeof(char) * arr_size);

    for(int i = 0; i < arr_size; i++){
        slice_arr[i] = arr[start + i];
    }
    return slice_arr;
}

void calculator_server(){
    // Handler
    struct sigaction act;
    void read_childproc();
    /* Variables within Send Random Server Port */
    int cal_sock;
    int random;
    struct sockaddr_in broad_adr;

    FILE* fp;
    char sendmsg[BUF_SIZE];
    char firstmsg[7] = "server:";
    char op[5];
    int so_brd = 1;

    /* Variables within Receive Approve */
    int recv_sock;
    struct sockaddr_in recv_adr;
    char recvmsg[BUF_SIZE];
    int rev_len;

    /* variable within Multiflexing server */
    int serv_sock, clnt_sock;
    fd_set reads, cpy_reads;
    struct sockaddr_in serv_adr, clnt_adr;
    struct timeval timeout;
    int fd_max, str_len, fd_num, i,j, len;

    /* variable within Calculating */
    char opCount;
    char buf[BUF_SIZE]; // sending operating line
    int operand[BUF_SIZE];
    char operator[BUF_SIZE];
    int result; // Result
    int cnt = 0;

    /* Handler Setting */
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    /* Calculation Server Sending and Getting */
    cal_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&broad_adr, 0, sizeof(broad_adr));
    broad_adr.sin_family = AF_INET;
    broad_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
    broad_adr.sin_port = htons(atoi("8080"));
    setsockopt(cal_sock, SOL_SOCKET, SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));

    /* Sending Random Port */
    random = (rand() % 40000) + 10001;
    fprintf(stdout, "Register calc server\n");
    memset(sendmsg, 0, sizeof(sendmsg));
    sprintf(sendmsg, "%s", firstmsg);
    sprintf(op, "%d", random);

    strcat(sendmsg, op);
    sendto(cal_sock, sendmsg, strlen(sendmsg), 0, (struct sockaddr*)&broad_adr, sizeof(broad_adr));
    
    /* Receive the message from Discovery Server */
    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);

    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_adr.sin_port = htons(atoi("8081"));

    if(bind(recv_sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr)) == -1)
        perror("bind()");
    memset(recvmsg, 0, sizeof(recvmsg));
    recvfrom(recv_sock, recvmsg, BUF_SIZE-1, 0, NULL, 0);
    if(!strcmp(recvmsg, "fail")){
        fprintf(stdout, "Fail\n");
        close(cal_sock);
        close(recv_sock);
        exit(0);
    }else{
        fprintf(stdout, "Calc Server(%s) operating...\n", recvmsg);
        if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror("socket creation failed");
            exit(1);
        }
        memset(&serv_adr, 0, sizeof(serv_adr));
        serv_adr.sin_family = AF_INET;
        serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_adr.sin_port = htons(atoi("8081"));

        if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
            perror("bind failed");
        if(listen(serv_sock, 5) == -1)
            perror("socket failed");
        FD_ZERO(&reads);
        FD_SET(serv_sock, &reads);
        fd_max = serv_sock;

        while(1){
            cpy_reads = reads;
            timeout.tv_sec = 5;
            timeout.tv_usec = 0;
            if((fd_num = select(fd_max + 1, &cpy_reads, 0, 0,&timeout)) == -1)
                break;
            if(fd_num == 0)
                continue;
            for(i =0;i<fd_max+1;i++){
                if(FD_ISSET(i, &cpy_reads)){
                    if(i==serv_sock){
                        len = sizeof(clnt_sock);
                        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &len);
                        FD_SET(clnt_sock, &reads);
                        if(fd_max < clnt_sock)
                            fd_max = clnt_sock;
                        printf("connected client: %d\n", clnt_sock);
                    }else{
                        memset(buf, 0, sizeof(buf));
                        cnt = 0;
                        str_len = read(i,buf,sizeof(buf));
                        if(str_len == 0){
                            FD_CLR(i,&reads);
                            close(i);
                            printf("closed client: %d\n",i);
                        }else{
                            sscanf(buf, "%c",&opCount);
                            cnt += sizeof(opCount);
                            if(opCount < 0 || opCount == 0){
                                sleep(5);
                                close(i);
                                fprintf(stdout, "closed client: %d\n",i);
                                exit(0);
                            }
                            for(j = 0;j<opCount;j++){
                                memcpy(&operand[j], buf+cnt, sizeof(operand[j]));
                                cnt += sizeof(operand[j]);
                            }
                            for(j=0;j<opCount-1;j++){
                                memcpy(&operator[j], buf+cnt, sizeof(operator[j]));
                                cnt += sizeof(operator[j]);
                            }
                            result = operand[0];
                            for(j=1;j<opCount;j++){
                                switch(operator[j-1])
                                {
                                    case'+':
                                        result += operand[j];
                                        break;
                                    case'-':
                                        result -= operand[j];
                                        break;
                                    case '*':
                                        result *= operand[j];
                                        break;
                                    default:
                                        printf("wrong value\n");
                                        exit(1);
                                }
                            }
                            write(clnt_sock, &result, 4);
                        }
                    }
                }
            }
        }
    }
    OPERATING = OFF;
    close(cal_sock);
}