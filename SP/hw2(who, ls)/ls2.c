#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <time.h>

void do_ls(char[]);
void do_ls_s(char[], int);
void dostat(char*);
void show_file_info(char*, struct stat*);
void mode_to_letters(int, char[]);
void sort_asc(struct dirent* [], long int[], int);
void sort_dec(struct dirent* [], long int[], int);
char* uid_to_name(uid_t);
char* gid_to_name(gid_t);

int main(int argc, char* argv[]) {
	struct stat info;
	if (argc == 1)
		do_ls(".");
	else{
		if(strcmp(argv[1],"-s")==0){
			printf("sort by size(ascending)\n");
			do_ls_s(".",0);
		}
		else if(strcmp(argv[1], "-S")==0){
			printf("sort bt size(descending)\n");
			do_ls_s(".",1);
		}
		else {
			printf("incorrect option\n");
			exit(1);
		}
	}
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

void do_ls_s(char dirname[], int type) {
	DIR* dir_ptr;
	struct dirent* direntp;
	struct dirent* direntp_arr[256];
	long int size_arr[256];
	struct stat* info;
	char path[256];
	if (chdir(dirname) != 0) {
		perror(dirname);
		exit(1);
	}
	getcwd(path, sizeof(path));
	if ((dir_ptr = opendir(path)) == NULL)
		fprintf(stderr, "ls2: cannot open %s\n", path);
	else {
		int i = 0;
		while ((direntp = readdir(dir_ptr)) != NULL) {
			if ((strcmp(direntp->d_name, ".") == 0) || (strcmp(direntp->d_name, "..") == 0))
				continue;
			direntp_arr[i] = direntp;
			stat(direntp->d_name, info);
			size_arr[i] = info->st_size;
			i++;
		}
		if (type == 0) sort_asc(direntp_arr, size_arr, i);
		else sort_dec(direntp_arr, size_arr, i);
		for(int k=0; k<i; k++)	dostat(direntp_arr[k]->d_name);
		closedir(dir_ptr);
	}
}

void sort_asc(struct dirent* ptr[], long int size[], int length) {
	int i, j, key;
	struct dirent* ptrkey;
	for (i = 1; i < j; i++) {
		key = size[i];
		ptrkey = ptr[i];
		for (j = i - 1; j >= 0 && size[j] > key; j--) {
			size[j + 1] = size[j];
			ptr[j + 1] = ptr[j];
		}
		size[j + 1] = key;
		ptr[j + 1] = ptrkey;
	}
}

void sort_dec(struct dirent* ptr[], long int size[], int length) {
	int i, j, key;
	struct dirent* ptrkey;
	for (i = 1; i < j; i++) {
		key = size[i];
		ptrkey = ptr[i];
		for (j = i - 1; j >= 0 && size[j] < key; j--) {
			size[j + 1] = size[j];
			ptr[j + 1] = ptr[j];
		}
		size[j + 1] = key;
		ptr[j + 1] = ptrkey;
	}
}

void dostat(char* filename) {
	struct stat info;
	if (stat(filename, &info) == -1)
		perror(filename);
	else
		show_file_info(filename, &info);
}

void show_file_info(char* filename, struct stat* info_p) {
	char* uid_to_name(), * ctime(), * gid_to_name(gid_t gid), * filemode();
	void mode_to_letters();
	char modestr[] = "-----------";

	mode_to_letters(info_p->st_mode, modestr);
	printf("%10s", modestr);
	printf("  %4d", (int)info_p->st_nlink);
	printf("  %-8s", uid_to_name(info_p->st_uid));
	printf("  %-10s", gid_to_name(info_p->st_gid));
	printf("  %-8ld", (long)info_p->st_size);
	printf("  %.12s", 4 + ctime(&info_p->st_mtime));
	printf("  %s \n", filename);
}

void mode_to_letters(int mode, char str[]) {
	if (S_ISDIR(mode)) str[0] = 'd';
	if (S_ISCHR(mode)) str[0] = 'c';
	if (S_ISBLK(mode)) str[0] = 'b';

	if (mode & S_IRUSR) str[1] = 'r';
	if (mode & S_IWUSR) str[2] = 'w';
	if (mode & S_IXUSR) str[3] = 'x';

	if (mode & S_IRGRP) str[4] = 'r';
	if (mode & S_IWGRP) str[5] = 'w';
	if (mode & S_IXGRP) str[6] = 'x';

	if (mode & S_IROTH) str[7] = 'r';
	if (mode & S_IWOTH) str[8] = 'w';
	if (mode & S_IXOTH) str[9] = 'x';
}

char* uid_to_name(uid_t uid) {
	struct passwd* getpwuid(), * pw_ptr;
	static char numstr[10];

	if ((pw_ptr = getpwuid(uid)) == NULL) {
		sprintf(numstr, "%d", uid);
		return numstr;
	}
	else
		return pw_ptr->pw_name;
}

char* gid_to_name(gid_t gid) {
	struct group* getgrgid(), * grp_ptr;
	static char numstr[10];

	if ((grp_ptr = getgrgid(gid)) == NULL) {
		sprintf(numstr, "%d", gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
}
