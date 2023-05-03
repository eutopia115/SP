#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

void oops(char *s1, char *s2);

int main(int ac, char *av[]){
	int in_fd, out_fd, n_chars;
	char buf[BUFFERSIZE];
	DIR* dir_ptr;
	struct dirent* direntp;
	char path[256];
	char x;
	
	if(ac<3)
		fprintf(stderr, "usage:%s -option source destination\n",*av);
	//(1)
	if(strcmp(av[1], "-I")==0) 	{
		if (chdir(".") != 0) {
		perror(".");
		exit(1);
		}
		getcwd(path, sizeof(path));
		if ((dir_ptr = opendir(path)) == NULL)
			fprintf(stderr, "ls2: cannot open %s\n", path);
		else {
			while ((direntp = readdir(dir_ptr)) != NULL) {
				if ((strcmp(direntp->d_name, ".") == 0) || (strcmp(direntp->d_name, "..") == 0))
					continue;
				if(strcmp(direntp->d_name, av[3])==0) {
					printf("파일을 덮어쓰시겠습니까? y/n : ");
					scanf("%c", &x);
					switch(x){
						case 'y' : break;
						case 'n' : exit(0);
					}
				}
			}
			closedir(dir_ptr);
		}
		if((in_fd=open(av[2],O_RDONLY)) == -1)
			oops("Cannot open ", av[1]);
		if((out_fd=creat(av[3], COPYMODE)) ==-1)
			oops("Cannot creat ", av[2]);
		while((n_chars=read(in_fd, buf, BUFFERSIZE)) > 0){
			if((write(out_fd, buf, n_chars)!=n_chars))	
				oops("write error to ", av[2]);
		}
		if(n_chars == -1)
			oops("read error from ", av[1]);
		if(close(in_fd)==-1||close(out_fd)==-1)
			oops("error closing file ", "");
	}
	//(2)
	else if(strcmp(av[1], "-P")==0){
		mode_t mode;
		struct stat st;
		stat(av[2], &st);
		if((in_fd=open(av[2],O_RDONLY)) == -1)
			oops("Cannot open ", av[1]);
		if((out_fd=creat(av[3], COPYMODE)) ==-1)
			oops("Cannot creat ", av[2]);
		mode=st.st_mode;
		while((n_chars=read(in_fd, buf, BUFFERSIZE)) > 0)
			if((write(out_fd, buf, n_chars)!=n_chars))	
				oops("write error to ", av[2]);
		chmod(av[3], mode);
		if(n_chars == -1)
			oops("read error from ", av[1]);
		if(close(in_fd)==-1||close(out_fd)==-1)
			oops("error closing file ", "");
	}
	//(3)
	else if(strcmp(av[1], "-PI")==0 || strcmp(av[1], "-IP")==0){
		mode_t mode;
		struct stat st;
		if (chdir(".") != 0) {
		perror(".");
		exit(1);
		}
		getcwd(path, sizeof(path));
		if ((dir_ptr = opendir(path)) == NULL)
			fprintf(stderr, "ls2: cannot open %s\n", path);
		else {
			while ((direntp = readdir(dir_ptr)) != NULL) {
				if ((strcmp(direntp->d_name, ".") == 0) || (strcmp(direntp->d_name, "..") == 0))
					continue;
				if(strcmp(direntp->d_name, av[3])==0) {
					printf("파일을 덮어쓰시겠습니까? y/n : ");
					scanf("%c", &x);
					switch(x){
						case 'y' : break;
						case 'n' : exit(0);
					}
				}
			}
			closedir(dir_ptr);
		}
		stat(av[2], &st);
		if((in_fd=open(av[2],O_RDONLY)) == -1)
			oops("Cannot open ", av[1]);
		if((out_fd=creat(av[3], COPYMODE)) ==-1)
			oops("Cannot creat ", av[2]);
		mode=st.st_mode;
		while((n_chars=read(in_fd, buf, BUFFERSIZE)) > 0)
			if((write(out_fd, buf, n_chars)!=n_chars))	
				oops("write error to ", av[2]);
		chmod(av[3], mode);
		if(n_chars == -1)
			oops("read error from ", av[1]);
		if(close(in_fd)==-1||close(out_fd)==-1)
			oops("error closing file ", "");
	}
	else printf("invaild option\n");
}


void oops(char *s1, char *s2){
	fprintf(stderr, "error:%s", s1);
	perror(s2);
	exit(1);
}
					       
