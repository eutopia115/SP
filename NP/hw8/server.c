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
#include <sys/uio.h>
#include <math.h>

#define usage(x) {printf("Usage : %s port\n",x); exit(1);}
#define err(x) {perror(x); exit(1);}
#define MAX_CLNT 10
#define QBS 8192/4

int c_cnt=0;
int c_socks[MAX_CLNT];
pthread_mutex_t mutex;

int operation(int x, int y, char op);
void* c_handle(void* arg);
void send_msg(char*msg, int len);

int main(int argc, char* argv[]){
    int s_sock, c_sock, c_addr_size; 
    struct sockaddr_in s_addr, c_addr;
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

int operation(int x, int y, char op){
    switch (op){
        case '+': return x + y;
        case '-': return x - y;
        case '*': return x * y;
    }
}

void* c_handle(void* arg){
    int c_sock=*((int*)arg);
    int len=0, opc, result, j=2, k=2;
    char id[5], rdbuf[QBS-1], op[QBS-1], optr[QBS], wtbuf[QBS*2], res[10], exp[QBS*2], tmp[10], *opcc;
    struct iovec vec[2];
    vec[0].iov_base = id;
    vec[0].iov_len = 6;
    vec[1].iov_base = rdbuf;
    vec[1].iov_len = QBS;
    while((len=readv(c_sock, vec, 2))!=0) {
        int opnd[QBS]={0,};
        opcc=strtok(rdbuf, " ");
        opc=atoi(opcc);
        printf("%d\n", opc);
        printf("%s\n", rdbuf);
        for(int i=0; i<opc; i++){
            opcc=strtok(NULL, " ");
            opnd[i]=atoi(opcc);
        }
        for(int i=0; i<opc-1; i++) {
            opcc=strtok(NULL, " ");
            optr[i]=opcc[0];
        }
        result=opnd[0];
        sprintf(exp, "%s %d", id, opnd[0]);
        for(int i=0; i<opc-1; i++) {
            sprintf(tmp, "%c%d", optr[i], opnd[i+1]);
            strcat(exp, tmp);
            result=operation(result, opnd[i+1], optr[i]);
        }
        sprintf(tmp, "%c%d\n", '=', result);
        strcat(exp, tmp);
        strcpy(wtbuf, exp);
        send_msg(wtbuf, strlen(wtbuf));
    }
    pthread_mutex_lock(&mutex);
    for(int i=0; i<c_cnt; i++){
        if(c_sock==c_socks[i]){
            while(i++<c_cnt-1) c_socks[i]=c_socks[i+1];
            break;
        }
    }
    c_cnt--;
    printf("closed client\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void send_msg(char*msg, int len){
    pthread_mutex_lock(&mutex);
    for(int i=0; i<c_cnt;i++) write(c_socks[i], msg, len);
    pthread_mutex_unlock(&mutex);
}
