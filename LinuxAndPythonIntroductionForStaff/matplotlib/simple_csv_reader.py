import sys
import numpy

if len(sys.argv) != 2:
   print 'Error: give a file name on the command line'
   exit(1) # Exit and return the error code 1

data = numpy.genfromtxt(sys.argv[1], dtype=float, delimiter=',', skip_header=True) 
print 'All data'
print data
