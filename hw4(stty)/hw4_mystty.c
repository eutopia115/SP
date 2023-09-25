#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

struct flaginfo{
	int fl_value;
	char *fl_name;
};

struct flaginfo input_flags[] = {
	IGNBRK, "IGNBRK",//"Ignore break conditon",
	BRKINT, "BRKINT",//"Signal interrup on break",
	IGNPAR, "IGNPAR", //"Ignore chars with parity errors",
	PARMRK, "PARMRK", //"Mark parity errors",
	INPCK, "INPCK",//"Enable input parity check",
	ISTRIP, "ISTRIP",//"Strip character",
	INLCR, "INLCR", //"Map NL to CR on input",
	IGNCR, "IGNCR",//"Ignore CR",
	ICRNL, "ICRNL",//"Map CR to NP on input",
	IXON, "IXON", //"Enable start/stop output control",
	IXOFF, "IXOFF",//"Enable start/stop input control",
	0, NULL
};

struct flaginfo output_flags[] = {
	OLCUC, "OLCUC",
	0, NULL
};

struct flaginfo local_flags[] = {
	ISIG, "ISIG", //"Enable signals",
	ICANON, "ICANON",//"Cannoical input(erase & kill)",
	ECHO, "ECHO", //"Enable echo",
	ECHOE, "ECHOE",//"Erase ERASE as BS-SPACE-BS",
	ECHOK, "ECHOK", //"Erase KILL by starting new line",
	0, NULL
};


void showbaud(int thespeed);
void show_some_flags(struct termios *ttyp);
void show_flagset(int thevalue, struct flaginfo thebitnames[]);
void execute(struct termios ttyinfo);
void flip(char* target, struct termios* info, int option);
void flipAll(struct termios* info);
unsigned int find(char* flname, int option);

int main(int argc, char *argv[]){
	struct termios ttyinfo;
	int option = -1;
	if(tcgetattr(0, &ttyinfo) == -1){
		perror("cannot get params about stdin");
		exit(1);
	}
	
	if(argc == 1) option = 0;
	else if(!strcmp(argv[1],"-FI")||!strcmp(argv[1],"-fi")) option = 1;
	else if(!strcmp(argv[1],"-FO")||!strcmp(argv[1],"-fo")) option = 2;
	else if(!strcmp(argv[1],"-FL")||!strcmp(argv[1],"-fl")) option = 3;
	else if(!strcmp(argv[1],"-A")||!strcmp(argv[1],"-a")) option = 4;

	switch(option){
		case 0 : execute(ttyinfo); break;
		case 1 : { //input
			for(int i=2; i<argc; i++){
				flip(argv[i], &ttyinfo, 1);
			}
			execute(ttyinfo);
			break;
		}
		case 2 : { //output
			flip("OLCUC", &ttyinfo, 2);
			execute(ttyinfo);
			break;
		}
		case 3 : { //local
			for(int i=2; i<argc; i++){
				flip(argv[i], &ttyinfo, 3);
			}
			execute(ttyinfo);
			break;
		}
		case 4 : { //all
			flipAll(&ttyinfo);
			execute(ttyinfo);
			break;
		}
		default : printf("Usage : ./mystty option argument1 argument2 ... \n"); break;
	}
}

void execute(struct termios ttyinfo){
	showbaud(cfgetospeed(&ttyinfo));
	printf("erase = ^%c   ", ttyinfo.c_cc[VERASE]-1+'A');
	printf("kill = ^%c\n", ttyinfo.c_cc[VKILL]-1+'A');
	show_some_flags(&ttyinfo);
}

void showbaud(int thespeed){
	printf("speed ");
	switch (thespeed){
	case B300: printf("300"); break;
	case B600: printf("600"); break;
	case B1200: printf("1200"); break;
	case B1800: printf("1800"); break;
	case B2400: printf("2400"); break;
	case B4800: printf("4800"); break;
	case B9600: printf("9600"); break;
	case B19200: printf("19200"); break;
	case B38400: printf("38400"); break;
	case B57600: printf("57600"); break;
	case B115200: printf("115200"); break;
	case B230400: printf("230400"); break;
	case B460800: printf("406800"); break;
	case B500000: printf("500000"); break;
	case B576000: printf("576000"); break;
	case B921600: printf("921600"); break;
	case B1152000: printf("1152000"); break;
	case B2500000: printf("2500000"); break;
	default: printf("fast");break;
	}
	printf(" baud\n");
}

void show_some_flags(struct termios *ttyp){
	show_flagset(ttyp->c_iflag, input_flags);
	show_flagset(ttyp->c_oflag, output_flags);
	show_flagset(ttyp->c_lflag, local_flags);
}

void show_flagset(int thevalue, struct flaginfo thebitnames[]){
	for(int i=0; thebitnames[i].fl_value; i++){
		if(!(thevalue & thebitnames[i].fl_value)) printf("-");
		printf("%s ", thebitnames[i].fl_name);
	}
	printf("\n");
}

unsigned int find(char* flname, int option){
	switch (option) {
	case 1 : for(int i=0; input_flags[i].fl_value; i++) 
		if(strcmp(input_flags[i].fl_name, flname)==0) return input_flags[i].fl_value;
	case 2 : if(strcmp(output_flags[0].fl_name, flname)==0) return output_flags[0].fl_value;
	case 3 : for(int i=0; local_flags[i].fl_value; i++)
		if(strcmp(local_flags[i].fl_name, flname)==0) return local_flags[i].fl_value;
	}
	printf("there is no flag equal to input, Please input flags in Captials\n");
	exit(1);
}


void flip(char* target, struct termios* info, int option){
	unsigned int targetInt = find(target, option);
	switch (option) {
	case 1: {
		if((info->c_iflag & targetInt)!=0) info->c_iflag &= ~targetInt;
		else info->c_iflag |= targetInt;
		break;
	}
	case 2: { 
		if((info->c_oflag & targetInt)!=0) info->c_oflag &= ~targetInt;
		else info->c_oflag |= targetInt;
		break;
	}
	case 3: {
		if((info->c_lflag & targetInt)!=0) info->c_lflag &= ~targetInt;
		else info->c_lflag |= targetInt;
		break;
	}
	default:{
		break;
	}
	}
if(tcsetattr(0, TCSANOW, info)== -1) printf("error\n");
}

void flipAll(struct termios* info){
	info->c_iflag = ~info->c_iflag;
	info->c_oflag = ~info->c_oflag;
	info->c_lflag = ~info->c_lflag;
	if(tcsetattr(0, TCSANOW, info)== -1) printf("error\n");
}
