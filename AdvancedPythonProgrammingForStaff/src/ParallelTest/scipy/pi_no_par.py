#!/usr/bin/env python
import sys
PI = 3.141592653589793

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
        #LOOP TO PARALLLELIZE
        h = 1.0/n
        sum = 0.0
        for i in range(1,n):
            x = h*(i - 0.5)
            sum += f(x)
        pi = h * sum
        #END
        sys.stdout.write("pi is approximatly: %.16f Error is: %.16f \n" %(pi, abs(pi-PI)));

if __name__ == "__main__":
    main()
