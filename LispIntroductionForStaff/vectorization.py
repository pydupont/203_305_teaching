import numpy

N = 1e4
a = numpy.random.random((N))
b = numpy.random.random((N))

c = numpy.zeros((N,N))

#print a, b, c

#for i in range(len(a)):
  #for j in range(len(b)):
    #c[i] = a[i] + b[j]
    
c = a + b