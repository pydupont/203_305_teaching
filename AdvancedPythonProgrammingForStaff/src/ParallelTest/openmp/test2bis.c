#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
int main (int argc, char const *argv[]){
  int n;
  int p = omp_get_num_procs();
  printf("Num proc: %d. Set num threads to: %d\n",p, p/3);
  // same as writing in the pragma #pragma omp parallel for num_threads(2)
  omp_set_num_threads(p/3);
  #pragma omp parallel for
  for(n=0;n<8;n++){
    printf("Element %d traité par le thread %d \n",n,omp_get_thread_num());
  }
  return EXIT_SUCCESS;
}
