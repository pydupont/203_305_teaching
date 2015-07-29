#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <mpi.h>
#define SIZE 1000
#define PROC 18

double get_time() {
  struct timeval tv;
  gettimeofday(&tv, (void *)0);
  return (double) tv.tv_sec + tv.tv_usec*1e-6;
}

double* alloc_mat(){
  return (double*) malloc(SIZE*SIZE*sizeof(double));
}

double* init_mat(){
  double* m =  alloc_mat();
  int i, j;
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      m[i*SIZE + j] = (double)rand()/(double)RAND_MAX;
    }
  }
  return m;
}

void multiply_mat(double* mat_A, double* mat_B, double* mat_res){
  int i , j, k;
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      mat_res[i*SIZE + j] = 0.0;
      for(k = 0; k < SIZE; k++){
	mat_res[i*SIZE + j] += (mat_A[i*SIZE + k]*mat_B[k*SIZE + j]);
      }
    }
  }
}

int main(int argc, char **argv){   
  MPI_Init(argc, argv);
  MPI_Barrier(MPI_COMM_WORLD);
  int nb;
  int rank = -1;
  double t,start,stop;
  double* mat_A;
  double* mat_B;
  double* mat_res = alloc_mat();
  // Allocations
  mat_A   = init_mat();
  mat_B   = init_mat();
  // Init
  printf("Nb.threads\tTps.\n");
  for(nb=1;nb<=PROC;nb++){
    MPI_Comm_size(MPI_COMM_WORLD, &nb);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    start = get_time();
    multiply_mat(mat_A, mat_B, mat_res);
    stop=get_time();
    t=stop-start;
    printf("%d\t%f\n",nb,t);
  }
  // Memory free
  free(mat_A);
  free(mat_B);
  free(mat_res);
  return EXIT_SUCCESS;
  MPI_Finalize(void);
}
