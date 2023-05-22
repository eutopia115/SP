#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/select.h>
#include <signal.h>
#include <sys/uio.h>

int operation(int x, int y, char op);
void chldhandler();

struct sigaction newhandler;

int sockfd, cSockfd;

int main(int argc, char* argv[]){
    struct sockaddr_in servaddr, cliaddr;
    struct timeval timeout;
    struct iovec vec[3];
    socklen_t len;
    fd_set reads, cpyReads;
    pid_t pid;
    int fdMax, strlen, fdNum, opCount, i, result, lenv, operand[BUFSIZ], fds1[2], fds2[2];
    char md[5], id[5], buf[BUFSIZ], operator[BUFSIZ], ansstr[BUFSIZ], tmp[5];
    
    vec[0].iov_base = md;
    vec[1].iov_base = id;
    vec[2].iov_base = buf;
    vec[0].iov_len = 4;
    vec[1].iov_len = 4;
    vec[2].iov_len = BUFSIZ;
    pipe(fds1);
    pipe(fds2);
    
    newhandler.sa_handler = chldhandler;
    sigemptyset(&newhandler.sa_mask);
    newhandler.sa_flags = 0;
    sigaction(SIGCHLD, &newhandler, NULL);

    if(argc<2){
        printf("usage : ./server localPort\n");
        return -1;
    }
    
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0))<0){
        perror("socket creation failed");
        return -1;
    }

    int enable =1;
    memset(&servaddr, 0, sizeof(servaddr));
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

    FD_ZERO(&reads);
    FD_SET(sockfd, &reads); 
    fdMax=sockfd;
    while(1){
        cpyReads=reads;
        timeout.tv_sec=5;
        timeout.tv_usec=5000;
        if((fdNum=select(fdMax+1, &cpyReads, 0, 0, &timeout))==-1) break;
        if(fdNum==0) continue;
        for(i=0; i<fdMax+1; i++){
            if(FD_ISSET(i, &cpyReads)){
                if(i==sockfd){
                    len=sizeof(cliaddr);
                    cSockfd=accept(sockfd,(struct sockaddr*)&cliaddr, &len);
                    FD_SET(cSockfd, &reads);
                    if(fdMax < cSockfd) fdMax=cSockfd;
                    printf("connected client : %d\n", cSockfd); 
                }
                else{
                    lenv=readv(cSockfd, vec, 3);
                    opCount=(int)buf[0];
                    for(int i=0; i<opCount; i++){
                        operand[i]=buf[(i*4)+1];
                        operand[i]=buf[(i*4+1)+1]<<8;
                        operand[i]=buf[(i*4+2)+1]<<16;
                        operand[i]=buf[(i*4+3)+1]<<24;
                    }
                    for(int i=0; i<opCount-1; i++){
                        operator[i]=buf[(opCount*4)+i+1];
                    }
                    if(lenv==0){
                        FD_CLR(i,&reads);
                        close(i);
                        printf("closed client : %d\n", i);   
                    }
                    else{
                        if(strcmp(md,"save")==0) {
                            pid=fork();
                            if(pid == 0){
                                close(sockfd);
                                int i=0;
                                FILE* fp = fopen("saved.txt","a+");;
                                char cldbuf[BUFSIZ];
                                read(fds1[0],cldbuf,BUFSIZ); 
                                for(i=0; cldbuf[i]!='\0'; i++);
                                fwrite(cldbuf,sizeof(char),i,fp);
                                fclose(fp);
                                printf("save to %c%c%c%c\n", cldbuf[0], cldbuf[1], cldbuf[2], cldbuf[3]);
                                close(cSockfd);
                                exit(0);
                            }
                            else {
                                result=operand[0];
                                for(int i=0; i<opCount-1; i++){
                                    result=operation(result, operand[i+1], operator[i]); 
                                }
                                sprintf(ansstr, "%s : " , id);
                                sprintf(tmp, "%d", operand[0]);
                                strcat(ansstr,tmp);
                                for(int i=0; i<opCount-1; i++){
                                    sprintf(tmp, "%c%d", operator[i], operand[i]);
                                    strcat(ansstr,tmp);
                                }
                                sprintf(tmp, "=%d\n", result);
                                strcat(ansstr,tmp);
                                write(fds1[1], ansstr, BUFSIZ);
                                write(cSockfd, &result, 4);
                                close(cSockfd);
                            }
                        }
                        else if(strcmp(md,"load")==0) {
                            pid=fork();
                            if(pid == 0){
                                close(sockfd);
                                int i=0, j=0;
                                FILE* fp = fopen("saved.txt","r");;
                                char cldbuf[BUFSIZ], tmp[BUFSIZ], givenid[5], line[BUFSIZ];
                                char ans[BUFSIZ] = "";
                                read(fds1[0], givenid, 4);
                                while(fgets(line,128,fp)){
                                    if(strstr(line,givenid)!=0)
                                        strcat(ans,line);
                                }
                                write(fds2[1],ans,BUFSIZ); 
                                fclose(fp);
                                printf("load from %s\n", givenid);
                                close(cSockfd);
                                exit(0);
                            }
                            else {
                                char parbuf[BUFSIZ];
                                write(fds1[1], id, 4);
                                read(fds2[0], parbuf, BUFSIZ);
                                write(cSockfd, parbuf, BUFSIZ);
                                printf("closed client : %d\n", cSockfd); 
                                close(cSockfd);
                            }
                        }
                        else if(strcmp(md,"quit")==0) {
                            pid=fork();
                            if(pid == 0){
                                close(sockfd);
                                close(cSockfd);
                                exit(0);
                            }
                            else close(cSockfd);   
                            }
                        }
                    }
                }
            }
        }
        close(sockfd);
    }
    


}

void chldhandler(){
    int status;
    pid_t pid=waitpid(-1, &status, WNOHANG);
}
