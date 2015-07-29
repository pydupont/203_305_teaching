#!/usr/bin/env python
# yop
def fibonacci(n):
    if type(n) != int or n < 0:
        print "n should be a positive number"
        exit(2)
    if n == 0: return 0
    if n == 1: return 1
    a = 0 #F0 = 0
    b = 1 #F1 = 1
    print "fibonacci(0) = %d" % a
    print "fibonacci(1) = %d" % b
    for i in range(2,n+1):
        f = a + b #Fn
        a = b     #Fn-2
        b = f     #Fn-1
        print "fibonacci(%d) = %d" % (i,f)
    return f

print fibonacci(7)

