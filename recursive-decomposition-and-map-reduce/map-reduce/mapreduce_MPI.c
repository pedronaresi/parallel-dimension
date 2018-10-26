#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 341425

//341426

int main(int argc, char **argv)
{
	int data[SIZE], i, last_code=0;
	int *counters;
	FILE *fp;

	if ((fp = fopen("data.txt","r")) == NULL) {
		printf("Can't open the input file: data.txt\n\n");
		exit(1);
	}
	for(i = 0; i < SIZE; i++){
		fscanf(fp,"%d\n", &data[i]);
		if (data[i]>last_code) last_code = data[i];
	}

	counters = (int*)malloc(sizeof(int) * last_code+1);
	for(i = 0; i < last_code+1; i++)
		counters[i]=0;

	//count
	for(i = 0; i < SIZE; i++)
		counters[data[i]]++;

	for(i = 0; i < last_code+1; i++)
		if(counters[i]!=0) printf("codigo %d: %d ocorrencias\n", i, counters[i]);
}
