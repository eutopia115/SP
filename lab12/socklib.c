#include "socklib.h"

#define HOSTLEN 256
#define BACKLOG 1

int make_server_socket(int portnum){
    return make_server_socket_q(portnum, BACKLOG);
}

int make_server_socket_q(int portnum, int backlog){
    struct sockaddr_in saddr;
    struct hostent* hp;
    char hostname[HOSTLEN];
    int sockid;
    
    if((sockid = socket(PF_INET, SOCK_STREAM, 0))==-1) return -1;
    bzero((void*)&saddr, sizeof(saddr));
    gethostname(hostname, HOSTLEN);
    hp = gethostbyname(hostname);
    bcopy((void*)hp->h_addr_list[0], (void*)&saddr.sin_addr, hp->h_length);
    saddr.sin_port = htons(portnum);
    saddr.sin_family = AF_INET;
    if(bind(sockid, (struct sockaddr*)&saddr, sizeof(saddr)) != 0) return -1;
    if(listen(sockid, backlog) != 0) return -1;
    return sockid; 
}

int connect_to_server(char* host, int portnum){
    int sock;
    struct sockaddr_in saddr;
    struct hostent* hp;

    if((sock = socket(PF_INET, SOCK_STREAM, 0))==-1) return -1;
    bzero((void*)&saddr, sizeof(saddr));
    if((hp = gethostbyname(host)) == NULL) return -1;
    bcopy(hp->h_addr_list[0], (struct sockaddr*)&saddr.sin_addr, hp->h_length);
    saddr.sin_port = htons(portnum);
    saddr.sin_family = AF_INET;
    if(connect(sock, (struct sockaddr*)&saddr, sizeof(saddr))!=0) return -1;
    return sock;
}