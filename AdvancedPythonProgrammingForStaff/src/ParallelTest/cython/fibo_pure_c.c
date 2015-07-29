#include <stdio.h>
#include <stdlib.h>

long fibo(long n){
  if(n == 0 || n == 1){return n;}
  return fibo(n - 2) + fibo(n - 1);
}


void main(int argc, char* argv[]){
  printf("%ld\n",fibo(38));
}
