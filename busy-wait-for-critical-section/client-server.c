//Algorithm Manna-Pnueli central server algorithm
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

//#define N 10000000 //10^7

#define MAX_THREADS 4 //2, 4

int request = 0, respond = 0;
long global = 0;

void critical_section(){
	global++;
}

void *client_process_i(void *tid) {
  long thid = (long) tid;

  while(true){
  	//non-critical section
  	while(respond != thid){
  		request = thid;
  	}
    sleep(1);
    printf("Thread %ld entrou na SC.\n", thid);

  	critical_section();
  	respond = 0;
    
    printf("Thread %ld saiu da SC.\n", thid);
    sleep(1);
  }
 
  pthread_exit(0);
}

void *server_process(void *tid) {
  long thid = (long) tid;
  
  while(true){
  	while(request == 0){ //await request != 0
    
  	}
  	respond = request;
  	while(respond != 0){ //await respond == 0
  	
  	}
  	request = 0;
  }

  pthread_exit(0);
} 

int main(void){
  pthread_t t_client[MAX_THREADS];
  pthread_t t_server;
  long th;

  pthread_create(&t_server, NULL, server_process, NULL);

  for(th=0; th<MAX_THREADS; th++){
    pthread_create(&t_client[th], NULL, client_process_i, (void *) th);
  }

  pthread_join(t_server, NULL);
  for(th=0; th<MAX_THREADS; th++) {
    pthread_join(t_client[th],NULL);
  }

  printf("%ld\n", global);

  return 0;
}
