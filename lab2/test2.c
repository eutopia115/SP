#include <stdio.h>
#include <stdlib.h>

int main(){
	int a;
	int b;
	int* arr;
	a=0;
	b=10;
	int sum=a+b;
	a=5;
	
	arr=(int*)malloc(sizeof(int)*(sum+1));
	arr[sum]=1;
	return 0;
	free(arr);
}
