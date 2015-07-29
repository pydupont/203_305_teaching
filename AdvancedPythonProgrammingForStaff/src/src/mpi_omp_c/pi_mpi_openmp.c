/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* *
* compute pi by intergrating f(x) = 4/(1 + x **2) *
* *
* Variables : *
* *
* pi the calculated result *
* n number of point of integration *
* x midpoint of each rectangle ’s interval *
* f function to integrate *
* sum , pi area of rectangles *
* tmp temporary scratch space for global summation *
* i loop index *
* *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>
#define PI 3.1415926535897932384626433832795029L

double f(double a) {
  return (double)4.0/((double)1.0+(a*a));
}

int main(int argc, char* argv[])
{
  int n, i;
  double h, pi, sum, x ;
  
  double mypi;
  int myid, numprocs;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  n = 0;
  for(;;) {
    if (myid == 0){
      printf("Enter the number of intervals: (0  quits)\n"); 
      if(!scanf("%d",&n)){return 2;}
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if(n ==0)
      break;
    //LOOP TO PARALLIZE
    h = ((double)1.0)/(double)n;
    sum = 0.0;
    #pragma omp parallel for reduction(+:sum) private(i,x)
    for(i = myid + 1; i <= n; i += numprocs){
      x = h * ((double)i - (double)0.5);
      sum += f(x);
    }
    mypi = h*sum;
    MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    //END
    if(myid == 0){
      printf("pi is approximatly: %.16f Error is: %.16f \n", pi, fabs(pi-PI));
    }
  }
  MPI_Finalize();
  return EXIT_SUCCESS ;
}
