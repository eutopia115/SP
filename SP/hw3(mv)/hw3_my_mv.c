#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h> 

#define BUFFERSIZE 4096
#define COPYMODE 0644
#define BUF_SIZ 1024

void mv(char dirname[], char dest[], char ext[]); 

char tmp1[256], tmp2[256];

int main(int argc, char *argv[5]){
	if(strcmp(argv[1],"-d") != 0) {
		fprintf(stderr, "destination directory does not exist\n");
		exit(1);
	}
	if(strcmp(argv[1],"-d") != 0 || strcmp(argv[3],"-e") != 0) {
		fprintf(stderr, "invaild option\n");
		fprintf(stderr, "usage:%s -d dest_dir -e file_extention\n",*argv);
		exit(1);
	}
	if(argc > 5) {
		fprintf(stderr, "usage:%s -d dest_dir -e file_extention\n",*argv);
		exit(1);
	}
	mv(".",argv[2], argv[4]);

	//mv(".","home/eutopia115/WorkSpace", "java");
}

void mv(char dirname[], char dest[], char ext[]) {
	DIR* dir_ptr, *check;
	struct dirent* direntp;
	char path[256];
	char buf[BUFFERSIZE];
	if (chdir(dirname) != 0) {
		perror(dirname);
		exit(1);
	}
	getcwd(path, sizeof(path));
	if ((dir_ptr = opendir(path)) == NULL)
		fprintf(stderr, "cannot open %s\n", path);
	strcat(tmp2,"./");
	strcat(tmp2,dest);
	if((check = opendir(tmp2)) == NULL){
		fprintf(stderr, "destination directory does not exist\n");
		exit(1);
	}
	else {
		int cnt=0;
		while ((direntp = readdir(dir_ptr)) != NULL) {
			if ((strcmp(direntp->d_name, ".") == 0) || (strcmp(direntp->d_name, "..") == 0) )	
				continue;
			else if(strstr(direntp->d_name, ext) != NULL){
				tmp1[0]='\0';
				tmp2[0]='\0';
				strcat(tmp1,"/");
				strcat(tmp2,"./");
				strcat(tmp1,direntp->d_name);
				strcat(tmp2,dest);
				strcat(tmp2,tmp1);
				rename(direntp->d_name,tmp2);
				cnt++;
			}	
		}
		if(cnt==0) {
			fprintf(stderr, "No files with extension %s found\n", ext);
			exit(1);
		}
	}
}
