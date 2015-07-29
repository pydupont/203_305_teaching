#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#define ITER 1000000000
#define PROC 18
double get_time() {
  struct timeval tv;
  gettimeofday(&tv, (void *)0);
  return (double) tv.tv_sec + tv.tv_usec*1e-6;
}
double calcul_pi_parallele(int nb_threads, int N){
  double start,stop;
  double cpu_time_used;
int i = 0;
  double valeur_pi = 0.0;
  double somme = 0.0;
  double fX;
  double h = (double)1.0 / (double)N;
  start = get_time();
  #pragma omp parallel for num_threads(nb_threads) private(fX) reduction(+:somme)
  for(i = 0; i < N; i++) {
    fX = h * (i - 0.5);
    /*#pragma omp critical
    {
      somme +=  4.0/(1.0 + fX*fX);
      }*/
    somme +=  4.0/(1.0 + fX*fX);
  }
  valeur_pi= h * somme;
  stop = get_time();
  printf("Valeur de pi %1.12f\n",valeur_pi);
  cpu_time_used = stop-start;
  return cpu_time_used;
}
int main(int argc, char **argv){   
  int nb;
  double t=0.0;
  printf("Nb.threads\tTps.\n");
  for(nb=1;nb<=PROC;nb++){
    t = calcul_pi_parallele(nb, ITER);
    printf("%d\t%f\n",nb,t);
  }
  return(EXIT_SUCCESS);
}
