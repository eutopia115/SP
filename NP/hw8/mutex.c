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

long long num = 0 ;
pthread_mutex_t mutex;

void* thread_inc(void* arg);
void* thread_dec(void* arg);

int main(int argc, char* argv[]){
    pthread_t threadid[10]; //[n] - number of thread
    pthread_mutex_init(&mutex, NULL);
    for(int i=0; i<10; i++){
        if(i%2) pthread_create(&(threadid[i]), NULL, thread_inc, NULL);
        else pthread_create(&(threadid[i]), NULL, thread_dec, NULL);
    }
    for(int i=0; i<10; i++) pthread_join(threadid[i], NULL);
    printf("result : %lld", num);
    pthread_mutex_destroy(&mutex);
    return 0;
}

void* thread_inc(void* arg){
    pthread_mutex_lock(&mutex);
    for(int i=0; i<50000; i++) num+=1;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* thread_dec(void* arg){
    pthread_mutex_lock(&mutex);
    for(int i=0; i<50000; i++) num-=1;
    pthread_mutex_unlock(&mutex);
    return NULL;
}