#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NUM 5

void main(){
    pthread_t t1, t2;
    void* print_msg(void*);
    pthread_create(&t1, NULL, print_msg, (void*) "Donghyeok SEO ");
    pthread_create(&t1, NULL, print_msg, (void*) "2021115360 \n");
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}

void* print_msg(void* m){
    char* cp = (char*) m;
    for(int i=0; i<NUM; i++){
        printf("%s", cp);
        fflush(stdout);
        sleep(1);
    }
    return NULL;
}