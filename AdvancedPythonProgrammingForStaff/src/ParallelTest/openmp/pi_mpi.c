#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mpi.h"
int size, rank; 
double start_time, end_time; 
void
xprintf(char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  printf("noeud %d: ", rank);
  vprintf(fmt, args);
}
void
xexit(int code)
{
  if ( rank == 0 ) {
    end_time = MPI_Wtime();
    xprintf("TEMPS DE CALCUL: "
      "%f secondes\n",
      end_time - start_time);
  }
  MPI_Finalize();
  exit(code);
}
double
f(double x)
{
  return (4.0 / (1.0 + x*x));
}
double
sub_sum(long long int start_i, 
        long long int stop_i, double h)
{
  double f_xi, f_xi1;
  long long int i = 0;
  double sum = 0;
  double x = 0;

  xprintf("Sous-somme de %Ld à %Ld\n",
	  start_i, stop_i);
  sum = 0;
  x = h*(start_i-1);
  f_xi1 = f(x);
  for ( i = start_i; i <= stop_i; i++ ) {
    f_xi = f_xi1;
    f_xi1 = f(x+h);
    sum += (f_xi+f_xi1);
    x += h;
  }
  sum *= h/2.0;
  return sum;
}
int
main(int argc, char **argv)
{
  long long int num_trap = 0;
  long long int i = 0, 
    start_i = 0, stop_i = 0;
  double h = 0;
  double sum = 0;
  double *sum_buf = NULL;
  size = rank = -1;
  alarm(600);

  MPI_Init(&argc, &argv);
  MPI_Barrier(MPI_COMM_WORLD);
  start_time = MPI_Wtime();
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  //before the computation
  if (rank == 0) {
    xprintf("Taille du cluster: %d\n",
            size);
    num_trap = atoll(argv[1]);
    xprintf("Nombre de trapèzes: %Ld\n",
           num_trap);
  }
  MPI_Bcast((void *) &num_trap, 1,
            MPI_LONG_LONG_INT, 0,
            MPI_COMM_WORLD);
  h = ( 1.0 - 0.0 ) / (double)num_trap;
  start_i = 1 + num_trap * rank / size;
  stop_i = num_trap * (rank + 1) / size;
  sum = sub_sum(start_i, stop_i, h);  
  if ( rank == 0 )
    sum_buf = malloc(sizeof(double) 
                     * size);
  MPI_Gather((void *)&sum, 1, 
             MPI_DOUBLE, sum_buf, 1,
             MPI_DOUBLE, 0,
             MPI_COMM_WORLD);
  if ( rank == 0 ) {
    sum = 0;
    for (i=0 ; i < size ; i++ ) {
      xprintf("somme=%.12f pour le "
	      "noeud %d\n",
	      sum_buf[i], i);
      sum += sum_buf[i];
    }
    xprintf("pi=%.12f\n", sum);
  }
  xexit(0);
  return 0;
}
