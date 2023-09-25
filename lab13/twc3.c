#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>

struct argSet{
    char* fname;
    int count;
};


void main(int argc, char* argv[]){
    pthread_t t1, t2;
    void* print_msg(void*);
    struct argSet args1, args2;
    if(argc!=3){printf("usage : %s file1 file2\n",argv[0]); exit(1);}
    args1.fname = argv[1];
    args2.fname = argv[2];
    args1.count = 0;
    args2.count = 0;
    pthread_create(&t1, NULL, print_msg, (void*)&args1);
    pthread_create(&t2, NULL, print_msg, (void*)&args2);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("%5d:%s\n", args1.count, argv[1]);
    printf("%5d:%s\n", args2.count, argv[2]);
    printf("%5d:total\n", args1.count+args2.count);
}

void* print_msg(void* m){
    struct argSet* args = (struct argSet*)m;
    FILE* fp;
    int c, prevc = '\0';
    if((fp=fopen(args->fname,"r"))!=NULL){
        while ((c=getc(fp))!=EOF) {
            if(!isalnum(c) && isalnum(prevc)) args->count++;
            prevc = c;
        }
        fclose(fp);
    }
    return NULL;
}