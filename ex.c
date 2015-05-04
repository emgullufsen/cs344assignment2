#include<stdlib.h>
#include<time.h>
#include<stdio.h>

int main(){
	srand(time(NULL));
	int i = rand() % 5;
	printf("i is %i\n", i);
	int j = 0;
	for (j; j < i; j++){
		printf("printing %i\n", j);
	}
	return 0;
}