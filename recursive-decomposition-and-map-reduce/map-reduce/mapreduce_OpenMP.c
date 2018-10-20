#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <omp.h>

#define SIZE 341425

int main(int argc, char **argv)
{
	int data[SIZE], i, last_code=0;
	int *counters;
	FILE *fp;

	struct timeval inicio, final;
  int tmili;

	omp_set_num_threads(1);

	if ((fp = fopen("data.txt","r")) == NULL) {
		printf("Can't open the input file: data.txt\n\n");
		exit(1);
	}

	gettimeofday(&inicio, NULL);

	#pragma parallel num_threads(4) private (i) reduction (||:last_code)
	{
		#pragma omp for
			for(i = 0; i < SIZE; i++){
				fscanf(fp,"%d\n", &data[i]);
				if (data[i]>last_code) last_code = data[i];
			}
	}

	counters = (int*)malloc(sizeof(int) * last_code+1);
	#pragma parallel num_threads(4) private (i) shared(counters[])
	{
		#pragma omp for
			for(i = 0; i < last_code+1; i++){
				counters[i]=0;
			}
	}

	//count
	#pragma parallel num_threads(4) private (i) shared(counters[])
	{
		#pragma omp for
			for(i = 0; i < SIZE; i++){
				counters[data[i]]++;
			}
	}

	gettimeofday(&final, NULL);
	tmili = (int) (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / 1000);

	for(i = 0; i < last_code+1; i++)
		if(counters[i]!=0) printf("codigo %d: %d ocorrencias\n", i, counters[i]);

	printf("O tempo paralelo foi de %dms\n", tmili);
}
