#include "stdio.h"
#include <stdlib.h>

#include <mpi.h>

#define SIZE 100
int main(int argc, char *argv[])
{
  int tid,nthreads;
  char *cpu_name;
  double time_initial,time_current,time;
  
  /* add in MPI startup routines */
  /* 1st: launch the MPI processes on each node */
  MPI_Init(&argc,&argv);
  time_initial  = MPI_Wtime();
  /* 2nd: request a thread id, sometimes called a "rank" from
          the MPI master process, which has rank or tid == 0
  */
  MPI_Comm_rank(MPI_COMM_WORLD, &tid);
  
  /* 3rd: this is often useful, get the number of threads
     or processes launched by MPI, this should be NCPUs-1
  */
  MPI_Comm_size(MPI_COMM_WORLD, &nthreads);
  
  int nb, i , j, k;
  double t,start,stop;
  double* mat_A;
  double* mat_B;
  double* mat_res;
  // Allocations
  mat_A   = (double*) malloc(SIZE*SIZE*sizeof(double)) ;
  mat_B   = (double*) malloc(SIZE*SIZE*sizeof(double)) ;
  mat_res = (double*) malloc(SIZE*SIZE*sizeof(double)) ;
  // Init
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      mat_A[i*SIZE + j] = (double)rand()/(double)RAND_MAX;
      mat_B[i*SIZE + j] = (double)rand()/(double)RAND_MAX;
    }
  }
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      mat_res[i*SIZE + j] = 0.0;
      for(k = 0; k < SIZE; k++){
	mat_res[i*SIZE + j] += (mat_A[i*SIZE + k]*mat_B[k*SIZE + j]);
      }
    }
  }
  /* on EVERY process, allocate space for the machine name */
  //cpu_name    = (char *)calloc(80,sizeof(char));
  
  /* get the machine name of this particular host ... well
     at least the first 80 characters of it ... */
  //gethostname(cpu_name,80);
  time_current  = MPI_Wtime();
  time  = time_current - time_initial;
  printf("%.3f\n", time);

  // Memory free
  free(mat_A);
  free(mat_B);
  free(mat_res);

  MPI_Finalize();
  return(0);
}
