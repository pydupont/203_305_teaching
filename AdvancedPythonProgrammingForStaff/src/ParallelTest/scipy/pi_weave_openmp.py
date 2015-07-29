#!/usr/bin/env python
import sys
from scipy import weave
from scipy.weave import converters
import time
PI = 3.141592653589793

code="""
int i;
double x;
double sum = 0.0;
#pragma omp parallel for private(i,x) reduction(+:sum)
for(i = 1;i <= n; i++) {
    x = h*((double)i-(double)0.5);
    sum += (double)4.0/((double)1.0+(x*x));
}
return_val = sum;
"""
vars = "h n".split()
weave_omp = \
{
    'headers': ['<omp.h>'],
    'extra_compile_args': ['-fopenmp'],
    'extra_link_args': ['-lgomp']
}

def f(a):
    return 4.0/(1.0+a**2)

def main():
    while(1):
        n = raw_input("Enter the number of intervals: (0  quits)\n")
        try:
            n=int(n)
        except ValueError:
            return 2
        if n == 0: break
        #LOOP TO PARALLIZE
        h = 1.0/n
        sum = float(0.0)
        tps = time.time()
        sum = weave.inline(code, vars, type_converters = converters.blitz, compiler = 'gcc', **weave_omp)
        print time.time() - tps
        pi = h * sum
        #END
        sys.stdout.write("pi is approximatly: %.16f Error is: %.16f \n" %(pi, abs(pi-PI)));

if __name__ == "__main__":
    main()
