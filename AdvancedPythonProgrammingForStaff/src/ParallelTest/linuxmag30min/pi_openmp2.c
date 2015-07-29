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
#include <omp.h>
#define PI 3.1415926535897932384626433832795029L

double f(double a) {
  return (double)4.0/((double)1.0+(a*a));
}

int main(int argc, char* argv[])
{
  int n, i;
  double h, pi, sum, x ;
  #pragma omp parallel private(i,x)
  for(;;) {
    #pragma omp single
    {
      printf("Enter the number of intervals: (0  quits)\n");
      if(!scanf("%u",&n)){
	return 2;
      }
    }
    if(n ==0)
      break;
    //LOOP TO PARALLIZE
    h = ((double)1.0)/(double)n;
    sum = 0.0;
    #pragma omp barrier
    #pragma omp for private(i,x) reduction(+:sum)
    for(i =1;i<=n;i++) {
      x = h*((double)i-(double)0.5);
      //#pragma omp critical
      sum += f(x);
    }
    #pragma omp single
    {
      pi = h*sum;
      //END
      printf("pi is approximatly: %.16f Error is: %.16f \n", pi, fabs(pi-PI));
    }
  }
  return EXIT_SUCCESS ;
}
