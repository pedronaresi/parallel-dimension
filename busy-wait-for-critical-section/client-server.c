//Algorithm Manna-Pnueli central server algorithm
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 10000000

#define MAX_THREADS 2 //2, 4

int request = 0, respond = 0;
int global = 0;

void critical_section(int x){
	global += x;
}

void *client_process_i(void *tid) {
  long thid = (long) tid;

  while(true){
  	//non-critical section
  	while(respond != thid){
  		request = thid;
  	}
  	critical_section(thid);
  	respond = 0;
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
  pthread_t t[MAX_THREADS];
  long i;
  long th;

  pthread_create(&t[0], NULL, server_process, (void *) 0);

  for(th=1; th<MAX_THREADS; th++) {
    pthread_create(&t[th], NULL, client_process_i, (void *) th);
  }

  /*
  for(th=0; th<MAX_THREADS; th++) {
    pthread_join(t[th],NULL);
  }*/

  return 0;
}
	