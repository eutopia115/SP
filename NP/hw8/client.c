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
#include <pthread.h>

#define usage(x) {printf("Usage : %s ip port name\n",x); exit(1);}
#define err(x) {perror(x); exit(1);}
#define MAX_CLNT 10
#define QBS 8192/4

int sock;
int c_cnt=0;
int c_socks[MAX_CLNT];
char name[QBS*2], msg[QBS*2];
pthread_mutex_t mutex;

void* send_msg(void* arg);
void* recv_msg(void* arg);
int overchecker(int opc);

int main(int argc, char* argv[]){
    struct sockaddr_in s_addr;
    void* thrd_ret;
    pthread_t send_thrd, recv_thrd;
    if(argc != 4) usage(argv[0]);
    pthread_mutex_init(&mutex, NULL);
    if(strlen(argv[3])!=4) {printf("id must be 4\n"); exit(1);}
    sprintf(name, "[%s]", argv[3]);
    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = inet_addr(argv[2]);
    s_addr.sin_port = htons(atoi(argv[1]));
    if(connect(sock, (struct sockaddr*)&s_addr, sizeof(s_addr))==-1) err("connect");
    pthread_create(&send_thrd, NULL, send_msg, (void*)&sock);
    pthread_create(&recv_thrd, NULL, recv_msg, (void*)&sock);
    pthread_join(send_thrd, &thrd_ret);
    pthread_join(recv_thrd, &thrd_ret);
    close(sock);
    return 0;
}

void* send_msg(void * arg){
    int sock=*((int*) arg);
    struct iovec vec[2];
    char* opcc, buf[QBS];
    int opc;
    while (1){
        fgets(msg, QBS*2, stdin);
        strcpy(buf,msg);
        opcc = strtok(msg," ");
        opc=atoi(opcc);
        if(opc>127 || opc<-128) {
            opc=overchecker(opc);
            printf("overflow number(%d) close client\n", opc); 
            exit(1);
        }
        if(!strcmp(msg,"q\n")||!strcmp(msg,"Q\n")){
            close(sock);
            exit(0);
        }
        vec[0].iov_base=name;
        vec[0].iov_len=6;
        vec[1].iov_base=buf;
        vec[1].iov_len=QBS;
        writev(sock, vec, 2);
    }
    return NULL;
}

void* recv_msg(void * arg){
    int sock=*((int*)arg);
    char buf[BUFSIZ];
    int len;
    while (1){
        len=read(sock, buf, BUFSIZ);
        if(len==-1) return (void*) -1;
        buf[len]=0;
        fputs(buf, stdout);
    }
    return NULL;
}

int overchecker(int opc){
    if(opc>127) opc-=256;
    if(opc<-128) opc+=256;
    if(opc>127 || opc<-128) overchecker(opc);
    return opc;
}