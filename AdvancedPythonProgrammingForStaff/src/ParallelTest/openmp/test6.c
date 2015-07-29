#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define MAX 1024

int main(int argc, char **argv){
  int id, nb, i, threads;
  char* file_name;
  if(argc != 3){
    printf("First argument should be number of threads;");
    printf(" the second, the path of the file containing the commands.\n");
    return 2;
  }
  nb = *argv[1];
  file_name = argv[2];
  printf("File name: %s\n", file_name);
  int size = 10;
  FILE* fil = fopen(file_name,"r");
  char ch[MAX] = "";
  if (fil != NULL){
    while (fgets(ch, MAX, fil) != NULL){
      printf("%s", ch);
    }
    fclose(fil);
  }
#pragma omp parallel for num_threads(nb) private(i) 
  for(i = 0; i < size; i++){
    id=omp_get_thread_num();
    threads=omp_get_num_threads();
    printf("%d, %d\n", id, threads);
  }
  return EXIT_SUCCESS;
}
