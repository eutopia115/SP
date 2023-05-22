#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/uio.h>


int main(int argc, char* argv[]){
    int sockfd, len;
    char md[BUFSIZ];
    char id[BUFSIZ];
    char buf[BUFSIZ];
    char tmp[BUFSIZ];
    int opndTmp;
    char oprTmp;
    struct sockaddr_in servaddr;
    struct iovec vec[3];
    int opCount, opResult;

    if(argc<3){
        printf("usage : ./client  remotePort remoteAddress\n");
        return -1;
    }
    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("socket creation failed");
        return -1;
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(argv[2]);
    servaddr.sin_port=htons(atoi(argv[1]));

    if(connect(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr))){
        perror("connect error");
        return -1;
    }

    printf("Mode : ");
    scanf(" %s", md);
    if(!(strcmp(md,"save")==0||strcmp(md,"load")==0||strcmp(md,"quit")==0)) {
        printf("supported mode : save load quit");
        exit(1);
    }
    else if(strcmp(md,"save")==0){
        printf("ID : ");
        scanf(" %s", id);
        len=strlen(id);
        if(len!=4){
            printf("Error : ID length must be 4\n");
            exit(1);
        }
        vec[0].iov_base = md;
        vec[0].iov_len = 4;
        vec[1].iov_base = id;
        vec[1].iov_len = 4;
        printf("Operand Count : ");
        scanf("%d", &opCount);
        buf[0]=(unsigned char)opCount;
        if(opCount!=buf[0]){
            printf("Overflow will happen(%d)", opCount);
            exit(1);
        }
        for(int i=0; i<opCount; i++){
            printf("Operand %d : ", i);
            scanf("%d",(int*)&buf[1+(i*4)]);
        }
        for(int i=0; i<opCount-1; i++){
            printf("Operator %d : ", i);
            scanf(" %c",&buf[(opCount*4+1)+i]);
        }
        vec[2].iov_base = buf;
        vec[2].iov_len = 1+(opCount)*4+(opCount-1);
        writev(sockfd, vec, 3);
        read(sockfd, &opResult, 4);
        if(opCount!=0) printf("Operation Result : %d\n", opResult);
    }
    else if(strcmp(md,"load")==0){
        char saved[BUFSIZ];
        printf("ID : ");
        scanf(" %s", id);
        len=strlen(id);
        if(len!=4){
            printf("Error : ID length must be 4\n");
            exit(1);
        }
        vec[0].iov_base = md;
        vec[0].iov_len = 4;
        vec[1].iov_base = id;
        vec[1].iov_len = 4;
        writev(sockfd, vec, 3);
        read(sockfd, saved, BUFSIZ);
        printf("%s", saved);
    }
    else if(strcmp(md,"quit")==0){
        vec[0].iov_base = md;
        vec[0].iov_len = 4;
        writev(sockfd, vec, 3);
    }
    close(sockfd);
    return 0;
}
