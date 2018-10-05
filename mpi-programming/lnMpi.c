#include <stdio.h>
#include <mpi.h>

#define N 100000000
#define u 2.0

int main(void) {
  double passo, soma,x;
  int i;

  int processId; /* rank dos processos */
  int noProcesses; /* Número de processos */
  int nameSize; /* Tamanho do nome */
  char computerName[MPI_MAX_PROCESSOR_NAME];

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &noProcesses);
  MPI_Comm_rank(MPI_COMM_WORLD, &processId);
  MPI_Get_processor_name(computerName, &nameSize);
  printf("Hello from process %d/%d on %s\n", processId,
  noProcesses, computerName);
  MPI_Finalize( );

  passo = (u-1) / (double) N;

  for (i=0; i<N;i++) {
    x=1+i*passo;
    soma=soma+0.5*(1/x+1/(x+passo));
  }

  printf("ln %f = %20.15f\n", u, passo*soma);
  return 0;
}
