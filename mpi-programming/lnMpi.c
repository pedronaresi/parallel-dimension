/* OBSERVAÇÃO:
	Não funciona para mpirun -n 1. Faltou apenas testar com if(nprocessos>1) antes de fazer o MPI_Send. 2) não funciona para mpirun -n 1. Faltou apenas testar com if	(nprocessos>1) antes de fazer o MPI_Send
*/

#include <stdio.h> 
#include "mpi.h"

#define N 1000
#define u 2.0

int main(int argc, char *argv[]){

	double passo, soma = 0, x, soma_parcial = 0.;
	int N_parcial, N_aux, particao;
	int i;

	int id, nprocessos; // nprocessos = size
	int ierr;
	ierr = MPI_Init(&argc, &argv); 
	ierr = MPI_Comm_size(MPI_COMM_WORLD, &nprocessos);
	ierr = MPI_Comm_rank(MPI_COMM_WORLD, &id);
	//MPI_STATUS status;

	particao = N / nprocessos;

	if(id == 0){
		N_parcial = particao;
		
		N_aux = 0;
		if(id < (N % nprocessos)) N_parcial + 1; // correcao de N_parcial
		ierr = MPI_Send(&N_parcial, 1, MPI_INTEGER, 1, 10, MPI_COMM_WORLD); // envia N / nprocessos pro processo 1
	}
	else{
		ierr = MPI_Recv(&N_parcial, 1, MPI_INTEGER, id-1, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recebe do processo id-1
		N_aux = N_parcial;
		N_parcial += particao;
		if(id < (N % nprocessos)) N_parcial + 1; // correcao de N_parcial
		if(id != nprocessos - 1){ // todos menos último processo
			ierr = MPI_Send(&N_parcial, 1, MPI_INTEGER, id+1, 10, MPI_COMM_WORLD); // envia pro processo i+1
		}
	}

	passo = (u-1.0) / (double) N;
	printf("id = %d --- passo = %f --- N_aux = %d --- N_parcial = %d\n", id, passo, N_aux, N_parcial);
	for(i = N_aux; i < N_parcial; i++){
		x = 1.0 + i * passo;
		soma_parcial = soma_parcial + 0.5 * ((1.0/x)+(1/(x+passo)));
	}
	printf("id = %d --- soma_parcial = %f\n", id, passo*soma_parcial);

	ierr = MPI_Barrier(MPI_COMM_WORLD);

	if(id != 0){
		ierr = MPI_Reduce(&soma_parcial, NULL, 1, MPI_DOUBLE,	MPI_SUM, 0, MPI_COMM_WORLD);
	}
	else{
		MPI_Reduce(&soma_parcial, &soma, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		printf("ln %f = %20.15f\n", u, passo*soma);
	}

	ierr = MPI_Finalize();
	return 0;
}
