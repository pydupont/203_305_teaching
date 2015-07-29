import matplotlib.pyplot as plt
import sys
import numpy

if len(sys.argv) != 2:
   print 'Error: give a file name on the command line'
   exit(1) # Exit and return the error code 1

data = numpy.genfromtxt(sys.argv[1], dtype=float, delimiter='\t', skip_header=True) 

plt.boxplot(data)
plt.ylim([-1,4])
plt.show()

