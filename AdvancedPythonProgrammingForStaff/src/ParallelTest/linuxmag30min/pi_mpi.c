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
#define PI 3.1415926535897932384626433832795029L

double f(double a) {
  return (double)4.0/((double)1.0+(a*a));
}

#define MTAG1 1
#define MTAG2 2

int main(int argc, char* argv[])
{
  int n, i;
  double h, pi, sum, x ;
  
  double mypi;
  int myid, numprocs, islave;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  for(;;) {
    if (myid == 0){
      printf("Enter the number of intervals: (0  quits)\n"); 
      if(!scanf("%d",&n)){return 2;}
      for(islave=1;islave<numprocs;islave++){
	MPI_Send(&n, 1, MPI_INT, islave, MTAG1, MPI_COMM_WORLD);
      }
    } else {
      MPI_Recv(&n, 1, MPI_INT, 0, MTAG1, MPI_COMM_WORLD, &status);
    }
    if(n ==0)
      break;
    //LOOP TO PARALLIZE
    h = ((double)1.0)/(double)n;
    sum = 0.0;
    for(i = myid + 1; i <= n; i += numprocs){
      x = h * ((double)i - (double)0.5);
      sum += f(x);
    }
    mypi = h*sum;
    if(myid != 0){
      MPI_Send(&mypi, 1, MPI_DOUBLE_PRECISION, 0, MTAG2, MPI_COMM_WORLD);
    } else {
      pi = mypi;
      for(islave = 1; islave <numprocs; islave++){
	MPI_Recv(&mypi, 1, MPI_DOUBLE_PRECISION, islave, MTAG2, MPI_COMM_WORLD, &status);
	pi += mypi;
      }
      //END
      printf("pi is approximatly: %.16f Error is: %.16f \n", pi, fabs(pi-PI));
    }
  }
  MPI_Finalize();
  return EXIT_SUCCESS ;
}
