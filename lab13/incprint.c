#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NUM 5

int counter = 0;

void main(){
    pthread_t t1;
    void* print_msg(void*);
    pthread_create(&t1, NULL, print_msg, NULL);
    for(int i=0; i<NUM; i++){
        counter++;
        sleep(1);
    }
    pthread_join(t1, NULL);
}

void* print_msg(void* m){
    for(int i=0; i<NUM; i++){
        printf("count = %d\n", counter);
        sleep(1);
    }
    return NULL;
}