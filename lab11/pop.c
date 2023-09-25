#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE *fp;
    char buf[BUFSIZ];
    int i=0;

    fp = popen("ls -l | sort | grep lab", "r");
    fread(buf, 1, BUFSIZ, fp);
    printf("%s",buf);
    pclose(fp);
    return 0;
}