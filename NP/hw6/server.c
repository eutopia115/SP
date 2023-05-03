//과제를 0을 받은 이후에도 클라이언트에는 계산결과가 출력되도록 하는 것으로 이해하여 코딩했습니다.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

int operation(int x, int y, char op);
void chldhandler();
int end=0;

struct sigaction newhandler;

int main(int argc, char* argv[]){
    int sockfd, cSockfd, opCount, result=0, operand[128], fds[2];
    unsigned char buf[1024], operator[127], opc, ansstr[1024], tmp[5];
    struct sockaddr_in servaddr, cliaddr;
    pid_t pid;
    socklen_t len;
    
    newhandler.sa_handler = chldhandler;
    sigemptyset(&newhandler.sa_mask);
    newhandler.sa_flags = 0;
    sigaction(SIGCHLD, &newhandler, NULL);

    if(argc<2){
        printf("usage : ./server localPort\n");
        return -1;
    }
    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("socket creation failed");
        return -1;
    }

    int enable =1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(atoi(argv[1]));

    if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0){
        perror("bind failed");
        return -1;
    }

    if(listen(sockfd, 5)<0){
        perror("socket failed");
        return -1;
    }
    while(1){
    if((cSockfd=accept(sockfd,(struct sockaddr*)&cliaddr, &len))<0){
        continue;
    }
    if(end==0) {
        printf("new client connected...\n"); 
        pipe(fds);
        pid=fork();
    }
    if(pid == -1) {close(cSockfd); continue;}
    if(pid == 0){
        close(sockfd);
        int i=0;
        FILE* fp = fopen("log.txt","a+");;
        char cldbuf[BUFSIZ];
        read(fds[0],cldbuf,BUFSIZ); 
        for(i=0; cldbuf[i]!='\0'; i++);
        if (cldbuf[0]=='x'){
            int tmp=0;
            tmp+=cldbuf[1];
            tmp+=cldbuf[2]*256;
            tmp+=cldbuf[3]*256*256;
            tmp+=cldbuf[4]*256*256*256;
            printf("Save File(%d)\n", tmp);
            close(cSockfd);
            exit(0);
        }
        fwrite(cldbuf,sizeof(char),i,fp);
        fclose(fp);
        close(cSockfd);
        exit(0);
    }
    else {
        read(cSockfd, &opc, 1);
        if(opc=='x') {
            read(cSockfd,&opCount,4);
            printf("Server Close(%d)\n", opCount);
            close(cSockfd);
            close(sockfd);
            exit(1);
        }
        opCount=(int)(opc);
        if(opCount<=0) {
            char tmpbuf[5];
            tmpbuf[0]='x';
            tmpbuf[1] = opCount;
            tmpbuf[2] = opCount >> 8 ;
            tmpbuf[3] = opCount >> 16 ;
            tmpbuf[4] = opCount >> 24 ;
            write(fds[1], tmpbuf, 5);
            end=1;
            close(cSockfd);
            continue;
        }
        for(int i=0; i<opCount; i++){
            read(cSockfd, &operand[i],4);
        }
        for(int i=0; i<opCount-1; i++){
            read(cSockfd, &operator[i],1);
        }
        result=operand[0];
        for(int i=0; i<opCount-1; i++){
            result=operation(result, operand[i+1], operator[i]); 
        }
        if(end==0){
            sprintf(ansstr, "%d : " , (int)pid);
            sprintf(tmp, "%d" ,operand[0]);
            strcat(ansstr,tmp);
            for(int i=0; i<opCount-1; i++){
                sprintf(tmp, "%c%d", operator[i], operand[i+1]);
                strcat(ansstr,tmp);
            }
            sprintf(tmp, "=%d\n", result);
            strcat(ansstr,tmp);
            printf("%s",ansstr);
            write(fds[1], ansstr, strlen(ansstr));
        }
        write(cSockfd, &result, 4);
        close(cSockfd);
    }
    }
    close(sockfd);
}

int operation(int x, int y, char op){
    switch (op){
        case '+': return x + y;
        case '-': return x - y;
        case '*': return x * y;
    }
}

void chldhandler(){
    int status;
    pid_t pid=waitpid(-1, &status, WNOHANG);
    printf("removed proc id : %d\n", pid);
}
