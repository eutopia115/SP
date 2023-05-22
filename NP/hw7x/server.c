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

int operation(int x, int y, char op);
void chldhandler();
void discMode();
void calcMode();
int stat;

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("usage : ./server <discovery or calc>\n");
        exit(1);
    }
    if(strcmp(argv[1],"discovery")==0) discMode();
    if(strcmp(argv[1], "calc")==0) calcMode();
}

void discMode(){
    int dis_sock, so_brd = 1, send_sock, clnt_sock, len;
    struct sockaddr_in broad_adr, send_adr, clnt_adr;
    char buf[BUFSIZ], randPort[BUFSIZ];
    char checker[BUFSIZ] = "server:";
    char fail[BUFSIZ] = "fail";
    
    dis_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&broad_adr, 0, sizeof(broad_adr));
    broad_adr.sin_family = AF_INET;
    broad_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    broad_adr.sin_port = htons(8080);
    
    printf("Discovery Server operating...\n");
    if(bind(dis_sock, (struct sockaddr*)&broad_adr, sizeof(broad_adr)) == -1){
        printf("bind error\n");
        exit(1);
    }


    while(1){
        len = recvfrom(dis_sock, buf, BUFSIZ-1, 0, NULL, 0);
        if(len < 0)
            break;
        if(strstr(buf, checker)!=NULL){
            send_sock = socket(PF_INET, SOCK_DGRAM, 0);
            memset(&send_adr, 0, sizeof(send_adr));
            send_adr.sin_family = AF_INET;
            send_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
            send_adr.sin_port = htons(8081);
            setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));
            if(stat){
                sendto(send_sock, "fail", strlen("fail"), 0, (struct sockaddr*)&broad_adr, sizeof(broad_adr));
                continue;
            }
            stat = 1;
            for(int i=7; i<12; i++) randPort[i-7]=buf[i];
            sendto(send_sock, randPort, strlen(randPort), 0, (struct sockaddr*)&send_adr, sizeof(send_adr));
            printf("Calc Server(%s) registerd\n", randPort);
            close(send_sock);    
        }
        else {
            clnt_sock = socket(PF_INET, SOCK_DGRAM, 0);
            memset(&send_adr, 0, sizeof(send_adr));
            clnt_adr.sin_family = AF_INET;
            clnt_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
            clnt_adr.sin_port = htons(8082);
            setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));
            if(stat) sendto(clnt_sock, randPort, strlen(randPort), 0, (struct sockaddr*)&clnt_adr, sizeof(clnt_adr));
            else sendto(clnt_sock, fail, strlen(fail), 0, (struct sockaddr*)&clnt_adr, sizeof(clnt_adr));
            close(clnt_sock);
        }
    }
    close(dis_sock);
}

void calcMode(){
    FILE* fp;
    struct timeval timeout;
    int cal_sock, recv_sock, serv_sock, clnt_sock, so_brd = 1, len, fd_max, fd_num, random;
    struct sockaddr_in broad_adr, recv_adr, serv_adr, clnt_adr;
    fd_set reads, cpy_reads;
    char randPort[BUFSIZ];
    
    char opCount, buf[BUFSIZ], operator[BUFSIZ];
    int operand[BUFSIZ], result;
    
    struct sigaction act;
    act.sa_handler = chldhandler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    cal_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&broad_adr, 0, sizeof(broad_adr));
    broad_adr.sin_family = AF_INET;
    broad_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
    broad_adr.sin_port = htons(8080);
    setsockopt(cal_sock, SOL_SOCKET, SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));

    random = (rand() % 40000) + 10001;
    printf("Register calc server...\n");
    sprintf(randPort, "server:%d", random);
    sendto(cal_sock, randPort, sizeof(randPort), 0, (struct sockaddr*)&broad_adr, sizeof(broad_adr));
    
    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_adr.sin_port = htons(8081);
    if(bind(recv_sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr)) == -1){
        printf("bind error\n");
        exit(1);
    }
    recvfrom(recv_sock, buf, BUFSIZ-1, 0, NULL, 0);
    if(strcmp(buf, "fail")==0){
        printf("Fail\n");
        close(cal_sock);
        close(recv_sock);
        exit(0);
    }else{
        printf("Calc Server(%s) operating...\n", buf);
        if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("socket creation failed");
            exit(1);
        }
        memset(&serv_adr, 0, sizeof(serv_adr));
        serv_adr.sin_family = AF_INET;
        serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_adr.sin_port = htons(8081);

        if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1){
            printf("bind error\n");
            exit(1);
        }
        if(listen(serv_sock, 5) == -1){
            printf("socket error\n");
            exit(1);
        }
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
            for(int i = 0; i<fd_max+1; i++){
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
                        len = read(i,buf,1);
                        if(len == 0){
                            FD_CLR(i,&reads);
                            close(i);
                            printf("closed client: %d\n",i);
                        }
                        else{
                            sscanf(buf, "%c",&opCount);
                            if(opCount < 0 || opCount == 0){
                                sleep(5);
                                close(i);
                                printf("closed client: %d\n",i);
                                exit(0);
                            }
                                for(int i=0; i<opCount; i++) read(clnt_sock, &operand[i],4);
                                for(int i=0; i<opCount-1; i++) read(clnt_sock, &operator[i],1);
                                result=operand[0];
                                for(int i=0; i<opCount-1; i++) result=operation(result, operand[i+1], operator[i]);
                            write(clnt_sock, &result, 4);
                        }
                    }
                }
            }
        }
    }
    stat = 0;
    close(cal_sock);
}

int operation(int x, int y, char op){
    switch (op){
        case '+': return x + y;
        case '-': return x - y;
        case '*': return x * y;
    }
}
void chldhandler() {
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        printf("Removed proc id: %d\n", pid);
    }
}