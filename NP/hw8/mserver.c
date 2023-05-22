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

#define usage(x) {printf("Usage : %s port\n",x); exit(1);}
#define err(x) {perror(x); exit(1);}
#define MAX_CLNT 10
#define QBS 8192/4

int c_cnt=0;
int c_socks[MAX_CLNT];
pthread_mutex_t mutex;

void* thread_inc(void* arg);
void* thread_dec(void* arg);
void* c_handle(void* arg);
void send_msg(char*msg, int len);

int main(int argc, char* argv[]){
    int s_sock, c_sock;
    struct sockaddr_in s_addr, c_addr;
    int c_addr_size;
    pthread_t t_id;
    if(argc != 2) usage(argv[0]);
    pthread_mutex_init(&mutex, NULL);
    s_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_addr.sin_port = htons(atoi(argv[1]));
    if(bind(s_sock, (struct sockaddr*)&s_addr, sizeof(s_addr))==-1) err("bind");
    if(listen(s_sock, 5)== -1) err("listen");
    while(1){
        c_addr_size = sizeof(c_addr);
        if((c_sock=accept(s_sock, (struct sockaddr*)&c_addr, &c_addr_size))<0) err("accept");
        pthread_mutex_lock(&mutex);
        c_socks[c_cnt++]=c_sock;
        pthread_mutex_unlock(&mutex);
        pthread_create(&t_id, NULL, c_handle, (void*)&c_sock);
        pthread_detach(t_id);
        printf("client ip : %s\n", inet_ntoa(c_addr.sin_addr));
    }
    close(s_sock);
    return 0;
}

void* c_handle(void* arg){
    int c_sock=*((int*)arg);
    int len=0;
    char msg[BUFSIZ];
    while((len=read(c_sock, msg, sizeof(msg)))!=0) send_msg(msg, len);
    pthread_mutex_lock(&mutex);
    for(int i=0; i<c_cnt; i++){
        if(c_sock==c_socks[i]){
            while(i++<c_cnt-1) c_socks[i]=c_socks[i+1];
            break;
        }
    }
    c_cnt--;
    pthread_mutex_unlock(&mutex);
    close(c_sock);
    return NULL;
}

void send_msg(char*msg, int len){
    pthread_mutex_lock(&mutex);
    for(int i=0; i<c_cnt;i++) write(c_socks[i], msg, len);
    pthread_mutex_unlock(&mutex);
}