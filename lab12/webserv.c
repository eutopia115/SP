#include "webserv.h"
#include "socklib.h"

int main(int argc, char* argv[]){
    int sock, fd;
    FILE* fpin;
    char req[BUFSIZ];

    if(argc == 1) {
        fprintf(stderr, "usage : %s portnum\n", argv[0]);
        exit(1);
    }

    if((sock = make_server_socket(atoi(argv[1]))) == -1) exit(2);

    while(1){
        fd = accept(sock, NULL, NULL);
        fpin = fdopen(fd, "r");
        fgets(req, BUFSIZ, fpin);
        printf("got a call : request = %s", req);
        read_til_crnl(fpin);
        process_rq(req, fd);
        fclose(fpin);
    }

    return 0;
}

void read_til_crnl(FILE* fp){
    char buf[BUFSIZ];
    while(fgets(buf, BUFSIZ, fp) != NULL && strcmp(buf, "\r\n") != 0);
}

void process_rq(char* rq, int fd){
    char cmd[BUFSIZ], arg[BUFSIZ];
    if(fork()!=0) return;
    strcpy(arg,"./");
    if(sscanf(rq,"%s%s", cmd, (arg+2)) != 2) return;
    if(strcmp(cmd,"GET")!=0) cannot_do(fd);
    else if(not_exist(arg)) do_404(arg, fd);
    else if(isadir(arg)) do_ls(arg,fd);
    else if(ends_in_cgi(arg)) do_exec(arg, fd);
    else do_cat(arg,fd);
}
void header(FILE* fp, char* type){
    fprintf(fp, "HTTP/1.0 200 OK\r\n");
    if(type) fprintf(fp, "Content Type : %s\r\n", type);
}
void cannot_do(int fd){
    FILE* fp = fdopen(fd, "w");
    fprintf(fp, "HTTP/1.0 404 Not Found\r\n");
    fprintf(fp, "Content Type : text/plain\r\n");
    fprintf(fp, "\r\n");
    fprintf(fp, "The command is not implemented\r\n");
    fclose(fp);
}
void do_404(char* item, int fd){
    FILE* fp = fdopen(fd, "w");
    fprintf(fp, "HTTP/1.0 404 Not Found\r\n");
    fprintf(fp, "Content Type : text/plain\r\n");
    fprintf(fp, "\r\n");
    fprintf(fp, "The item you requested : %s\r\n is Not Found\r\n", item);
    fclose(fp);
}
int isadir(char* f){
    struct stat info;
    return (stat(f, &info)!= -1 && S_ISDIR(info.st_mode));
}
int not_exist(char* f){
    struct stat info;
    return (stat(f, &info)== -1);
    
}
char* file_type(char* f){
    char *cp;
    if((cp=strchr(f, '.')) != NULL) return cp+1;
    return "";
}
int ends_in_cgi(char* f){
    return (strcmp(file_type(f), "cgi")==0);
}

void do_ls(char* dir, int fd){
    FILE* fp;
    fp = fdopen(fd, "w");
    fprintf(fp, "\r\n");
    fflush(fp);
    dup2(fd, 1);
    dup2(fd, 2);
    close(fd);
    execlp("ls", "ls", "-l", dir, NULL);
    perror(dir);
    exit(1);
}
void do_exec(char* prog, int fd){
    FILE* fp;
    fp = fdopen(fd,"w");
    header(fp, NULL);
    fflush(fp);
    dup2(fd, 1);
    dup2(fd, 2);
    close(fd);
    execl(prog, prog, NULL);
    perror(prog);
}

void do_cat(char* f, int fd){
    char* ext = file_type(f);
    char* content = "text/plain";
    FILE *fpsock, *fpfile;
    int c;

    if(strcmp(ext, "html")==0) content = "text/html";
    else if(strcmp(ext, "gif")==0) content = "image/gif";
    else if(strcmp(ext, "jpg")==0) content = "image/jpg";
    else if(strcmp(ext, "jpeg")==0) content = "image/jpeg";

    fpsock = fdopen(fd, "w");
    fpfile = fopen(f, "r");

    if(fpsock != NULL && fpfile != NULL){
        header(fpsock, content);
        fprintf(fpsock, "\r\n");
        while((c=getc(fpfile))!= EOF) putc(c, fpsock);
        fclose(fpsock);
        fclose(fpfile);
    }
}