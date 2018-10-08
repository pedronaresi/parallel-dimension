#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define tam 1.0
#define dx 0.00001
#define dt 0.000001
#define T  0.01
#define kappa 0.000045

int main(int argc, char *argv[]){

	double *tmp, *u, *u_prev;
  	double x, t;
  	long int i, n, maxloc;
  	double i_menos1, i_mais1;
  	long int n_parcial, n_aux;
  	
  	long int particao_n;

  	long int t_aux = 1;

  	double max;

  	double aux1;
  	int aux2;

  	int j;

	int id, nprocessos;
	int ierr;
	//MPI_STATUS status;
	ierr = MPI_Init(&argc, &argv); 
	ierr = MPI_Comm_size(MPI_COMM_WORLD, &nprocessos);
	ierr = MPI_Comm_rank(MPI_COMM_WORLD, &id);

	n = tam / dx;

	if(id == 0){
    printf("Inicio: qtde=%ld, dt=%g, dx=%g, dx²=%g, kappa=%f, const=%f\n", 
  	 (n+ 1), dt, dx, dx*dx, kappa, kappa*dt/(dx*dx));
    printf("Iteracoes previstas: %g\n", T/dt);
  }

	// Alocando os vetores
	/*
	u = (double *) malloc((particao_n)*sizeof(double));
  	u_prev = (double *) malloc((particao_n)*sizeof(double));
  	*/
  	//particao_t = ((int)(T / dt)) / nprocessos;
/*
  	if(id == 0){
  		printf("Inicio: qtde=%ld, dt=%g, dx=%g, dx²=%g, kappa=%f, const=%f\n", 
	 		(n+1), dt, dx, dx*dx, kappa, kappa*dt/(dx*dx));
  		printf("Iteracoes previstas: %g\n", T/dt);

  		x = 0;
  		t = 0.;

  		n_aux = 0;
  		n_parcial = particao_n;

  		t_aux = t;
  		T_parcial = particao_t;

  		if(id < (n+1) % nprocessos) n_parcial + 1;

  		ierr = MPI_Send(&particao_n, 1, MPI_INTEGER, 1, 10, MPI_COMM_WORLD);
  		ierr = MPI_Send(&particao_t, 1, MPI_DOUBLE, 1, 20, MPI_COMM_WORLD);
  	}
  	else{
  		ierr = MPI_Recv(&n_aux, 1, MPI_INTEGER, id-1, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  		ierr = MPI_Recv(&t_aux, 1, MPI_DOUBLE, id-1, 20, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  		n_parcial = n_aux + particao_n;
  		T_parcial = t_aux + particao_t;

  		if(id < (n+1) % nprocessos) n_parcial + 1;

  		if(id != nprocessos-1){
  			ierr = MPI_Send(&particao_n, 1, MPI_INTEGER, id+1, 10, MPI_COMM_WORLD);
  			ierr = MPI_Send(&particao_t, 1, MPI_DOUBLE, id+1, 20, MPI_COMM_WORLD);
  		}
  		else{
  			n_parcial++;
  		}
  	}
*/
	particao_n = n-1 / nprocessos;
  	if(id < n-1 % particao_n) particao_n + 1;
  	//if(id == nprocessos-1) particao_n + 1;

  	u = (double *) malloc((particao_n)*sizeof(double));
  	u_prev = (double *) malloc((particao_n)*sizeof(double));

  	x = particao_n * id * dx;
 	//x = 0.4;
  	for(i = 0; i < particao_n; i++){
  		if (x<=0.5) u_prev[i] = 200*x;
    	else        u_prev[i] = 200*(1.-x);
    	x += dx;
  	}

  	/*
  	// enviando u_prev[i-1]
  	if(id == 0){
  		ierr = MPI_Send(&u_prev[n_parcial-1], 1, MPI_DOUBLE, 1, 30, MPI_COMM_WORLD);
  	}
  	else{
  		ierr = MPI_Recv(&u_prev[n_aux-1], 1, MPI_DOUBLE, id-1, 30, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  		if(id != nprocessos-1){
  			ierr = MPI_Send(&u_prev[n_aux-1], 1, MPI_DOUBLE, id+1, 30, MPI_COMM_WORLD);
  		}
  	}

  	// enviando u_prev[i+1]
  	if(id == nprocessos-1){
  		ierr = MPI_Send(&u_prev[n_aux], 1, MPI_DOUBLE, id-1, 40, MPI_COMM_WORLD);
  	}
  	else{
  		ierr = MPI_Recv(&u_prev[n_parcial], 1, MPI_DOUBLE, id+1, 40, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  		if(id != 0){
  			ierr = MPI_Send(&u_prev[n_aux], 1, MPI_DOUBLE, id-1, 40, MPI_COMM_WORLD);
  		}
  	}
	*/

	if(id == 0){
		ierr = MPI_Send(&u_prev[particao_n-1], 1, MPI_DOUBLE, 1, 30, MPI_COMM_WORLD);
		i_menos1 = 0;
	}
	else{
		ierr = MPI_Recv(&i_menos1, 1, MPI_DOUBLE, id-1, 30, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if(id != nprocessos-1){
  			ierr = MPI_Send(&u_prev[particao_n-1], 1, MPI_DOUBLE, id+1, 30, MPI_COMM_WORLD);
  		}
	}

	if(id == nprocessos-1){
  		ierr = MPI_Send(&u_prev[0], 1, MPI_DOUBLE, id-1, 40, MPI_COMM_WORLD);
  		i_mais1 = (x <= 0.5) ? 200*x : 200*(1.-x);
  	}
  	else{
  		ierr = MPI_Recv(&i_mais1, 1, MPI_DOUBLE, id+1, 40, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  		if(id != 0){
  			ierr = MPI_Send(&u_prev[0], 1, MPI_DOUBLE, id-1, 40, MPI_COMM_WORLD);
  		}
  	}

  	printf("i_menos1 = %f --- i_mais1 = %f\n", i_menos1, i_mais1);

  	t = 0.;

  	while(t<T){
  		x = dx;
  		u[0] = u_prev[0] + kappa*dt/(dx*dx)*(i_menos1-2*u_prev[0]+u_prev[1]);

  		for(i = 1; i < particao_n-1; i++){
  			u[i] = u_prev[i] + kappa*dt/(dx*dx)*(u_prev[i-1]-2*u_prev[i]+u_prev[i+1]);
      		x += dx;
  		}
  		u[particao_n-1] = u_prev[particao_n-1] + kappa*dt/(dx*dx)*(u_prev[particao_n-2]-2*u_prev[particao_n-1]+i_mais1);
  		
  		tmp = u_prev; u_prev = u; u = tmp;

  	if(id !=0  && id != nprocessos - 1){ 
	    MPI_Send(&u_prev[particao_n - 1], 1, MPI_DOUBLE, id + 1, t_aux, MPI_COMM_WORLD);
	    MPI_Recv(&i_menos1, 1, MPI_DOUBLE, id - 1, t_aux, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    MPI_Send(u_prev, 1, MPI_DOUBLE, id -1, t_aux, MPI_COMM_WORLD);
	    MPI_Recv(&i_mais1, 1, MPI_DOUBLE, id + 1, t_aux, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else{
      if(id == 0){
        i_menos1 = 0;
        if(id != nprocessos - 1){
            MPI_Send(&u_prev[particao_n-1], 1, MPI_DOUBLE, id + 1, t_aux, MPI_COMM_WORLD);
            MPI_Recv(&i_mais1, 1, MPI_DOUBLE, id + 1, t_aux, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
      }
      if(id == nprocessos - 1){
        i_mais1 = 0;
        if(id != 0){
          	MPI_Recv(&i_menos1, 1, MPI_DOUBLE, id - 1, t_aux, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          	MPI_Send(u_prev, 1, MPI_DOUBLE, id -1, t_aux, MPI_COMM_WORLD);
        }
      } 
    }
    	t_aux++;
  		t += dt;
  	}

  	maxloc = 0;
  	for(i = 0; i < particao_n; i++){
  		if(u[i] > u[maxloc]){
  			maxloc = i;
  		}
  	}
  	max = u[maxloc];
  	if(id != 0){
  		MPI_Send(&u[maxloc], 1, MPI_DOUBLE, 0, 110, MPI_COMM_WORLD);
  		MPI_Send(&maxloc, 1, MPI_INTEGER, 0, 120, MPI_COMM_WORLD);
  	}
  	else{
  		
  		for(i = 1; i < nprocessos; i++){
  			MPI_Recv(&aux1, 1, MPI_DOUBLE, i, 110, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  			MPI_Recv(&aux2, 1, MPI_INTEGER, i, 120, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  			if(aux1 > max){
  				max = aux1;
  				maxloc = i;
  			}
  		}
  		printf("Maior valor u[%d] = %f\n", maxloc, max);
  	}

  	ierr = MPI_Finalize();
	return 0;
}
