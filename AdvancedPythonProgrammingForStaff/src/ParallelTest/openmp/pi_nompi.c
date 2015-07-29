#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

double
f(double x)
{
  return ( 4.0 / (1.0 + x*x) );
}
int
main(int argc, char **argv)
{
  long long int num_trap = 0;
  long long int i = 0;
  double x = 0;
  double h = 0;
  double sum = 0;
  alarm(600);
  num_trap = atoll(argv[1]);
  printf("Nombre de trapèzes: %Ld\n",
	 num_trap);
  h = 1 / (long double)num_trap;
  x = 0;
  sum = 0;
  for ( i = 2; i < num_trap; i++) {
    sum += f(x) * h;
    x += h;
  }
  sum += (f(0) + (f(1))) * h 
    / ( (long double) 2);
  printf("pi=%.12f\n", sum);
  return 0;
}
