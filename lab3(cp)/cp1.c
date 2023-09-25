#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

void oops(char *s1, char *s2);

int main(int ac, char *av[]){
	int in_fd, out_fd, n_chars;
	char buf[BUFFERSIZE];
	
	if(ac != 3){
		fprintf(stderr, "usage:%s source destination\n",*av);
		exit(1);
	}
	
	
	if((in_fd=open(av[1],O_RDONLY)) == -1)
		oops("Cannot open ", av[1]);
	if((out_fd=creat(av[2], COPYMODE)) ==-1)
		oops("Cannot creat ", av[2]);
	while((n_chars=read(in_fd, buf, BUFFERSIZE)) > 0)
		if((write(out_fd, buf, n_chars)!=n_chars))	
			oops("write error to ", av[2]);
	if(n_chars == -1)
		oops("read error from ", av[1]);
	if(close(in_fd)==-1||close(out_fd)==-1)
		oops("error closing file ", "");
}

void do_ls(char dirname[]) {
	DIR* dir_ptr;
	struct dirent* direntp;
	char path[256];
	if (chdir(dirname) != 0) {
		perror(dirname);
		exit(1);
	}
	getcwd(path, sizeof(path));
	if ((dir_ptr = opendir(path)) == NULL)
		fprintf(stderr, "ls2: cannot open %s\n", path);
	else {
		while ((direntp = readdir(dir_ptr)) != NULL) {
			if ((strcmp(direntp->d_name, ".") == 0) || (strcmp(direntp->d_name, "..") == 0))
				continue;
			dostat(direntp->d_name);
		}
		closedir(dir_ptr);
	}
}

void oops(char *s1, char *s2){
	fprintf(stderr, "error:%s", s1);
	perror(s2);
	exit(1);
}
					       
