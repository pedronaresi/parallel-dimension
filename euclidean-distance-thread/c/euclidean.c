#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define N 10000000 //10^5 / 10^7

#define MAX_THREADS 2 //1, 2, 4, 8


double *r1, *r2, *teta1, *teta2;

double *result;

double *maior;

void *distancia_maxima(void *tid) {
  long thid;
  long i;

  thid = (long) tid;

  maior[thid] = result[thid];

  for(i=thid; i<N; i+=MAX_THREADS){
      if (result[i]>maior[thid])
        maior[thid] = result[i];
  }
  printf("Thread_dist_max %ld finalizou.\n", thid);

  pthread_exit(0);
}

void *distancia_radial(void *tid) {
  long thid;
  long i;

  thid = (long) tid;

  for(i=thid; i<N; i+=MAX_THREADS){
      result[i] = sqrt((pow(r1[i],2) + pow(r2[i],2) - 2*r1[i]*r2[i]*cos(teta1[i]-teta2[i])));
  }

  printf("Thread_radial %ld finalizou.\n", thid);

  pthread_exit(0);
} 

int main(void){
  pthread_t t[MAX_THREADS];
  double maxglobal;
  long i;
  long th;

  struct timeval inicio, final2;
  int tmili;

  r1 = (double*) malloc(N*sizeof(double));
  r2 = (double*) malloc(N*sizeof(double));
  teta1 = (double*) malloc(N*sizeof(double));
  teta2 = (double*) malloc(N*sizeof(double));

  result = (double*) malloc(N*sizeof(double));

  maior = (double*) malloc(MAX_THREADS*sizeof(double));

  for(i=0; i<N; i++){
      r1[i] = (double) (rand()%100);
      r2[i] = (double) (rand()%100);
  }

  for(i=0; i<N; i++){
      teta1[i] = (double) (rand()%360);
      teta2[i] = (double) (rand()%360);
  }

  gettimeofday(&inicio, NULL);

  for(th=0; th<MAX_THREADS; th++) {
    pthread_create(&t[th], NULL, distancia_radial, (void *) th);
  }

  for(th=0; th<MAX_THREADS; th++) {
    pthread_join(t[th],NULL);
  }

  gettimeofday(&final2, NULL);
  tmili = (int) (1000 * (final2.tv_sec - inicio.tv_sec) + (final2.tv_usec - inicio.tv_usec) / 1000);

  printf("\n>Parte1: tempo decorrido para o calculo: %d milisegundos\n\n", tmili);

  //Verificacao do maior
  
  gettimeofday(&inicio, NULL);

  for(th=0; th<MAX_THREADS; th++) {
    pthread_create(&t[th], NULL, distancia_maxima, (void *) th);
  }

  for(th=0; th<MAX_THREADS; th++) {
    pthread_join(t[th],NULL);
  }

  for(th=1; th<MAX_THREADS; th++) {
    if (maior[0]<maior[th]) maior[0]=maior[th];
  }

  gettimeofday(&final2, NULL);
  tmili = (int) (1000 * (final2.tv_sec - inicio.tv_sec) + (final2.tv_usec - inicio.tv_usec) / 1000);

  printf("\n>Parte2: tempo decorrido para o calculo: %d milisegundos\n", tmili);

  printf("Distancia maxima: %f\n", maior[0]);
  
  /*
  for(i=0; i<N; i++){
      printf("%ld: %f\n", i, result[i]);
  }*/

  return 0;
}
