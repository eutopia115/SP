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

int main(int argc, char* argv[]){

    struct sockaddr_in s_addr;
    void* thrd_ret;
    pthread_t send_thrd, recv_thrd;
    if(argc != 4) usage(argv[0]);
    pthread_mutex_init(&mutex, NULL);
    sprintf(name, "[%s]", argv[3]);
    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = inet_addr(argv[1]);
    s_addr.sin_port = htons(atoi(argv[2]));
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
    char name_msg[BUFSIZ];
    while (1){
        fgets(msg, QBS*2, stdin);
        if(!strcmp(msg,"q\n")||!strcmp(msg,"Q\n")){
            close(sock);
            exit(0);
        }
        sprintf(name_msg,"%s %s", name, msg);
        write(sock, name_msg, strlen(name_msg));
    }
    return NULL;
}

void* recv_msg(void * arg){
    int sock=*((int*)arg);
    char name_msg[BUFSIZ];
    int len;
    while (1){
        len=read(sock, name_msg, BUFSIZ);
        if(len==-1) return (void*) -1;
        name_msg[len]=0;
        fputs(name_msg, stdout);
    }
    return NULL;
}