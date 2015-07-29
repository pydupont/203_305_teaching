#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#define SIZE 1500

double get_time() {
  struct timeval tv;
  gettimeofday(&tv, (void *)0);
  return (double) tv.tv_sec + tv.tv_usec*1e-6;
}

int main(int argc, char **argv){   
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
  printf("Nb.threads\tTps.\n");
  for(nb=1;nb<=18;nb++){
    start = get_time();
    
    #pragma omp parallel for num_threads(nb) private(j,k) 
    for(i = 0; i < SIZE; i++){
      for(j = 0; j < SIZE; j++){
        mat_res[i*SIZE + j] = 0.0;
        for(k = 0; k < SIZE; k++){
          mat_res[i*SIZE + j] += (mat_A[i*SIZE + k]*mat_B[k*SIZE + j]);
        }
     }
    }
    stop=get_time();
    t=stop-start;
    printf("%d\t%f\n",nb,t);
  }
  // Memory free
  free(mat_A);
  free(mat_B);
  free(mat_res);
  return EXIT_SUCCESS;
}
