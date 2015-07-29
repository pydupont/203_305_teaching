#!/usr/bin/env python
import numpy as np
import sys

if len(sys.argv) != 3:
    print "Please give an input file and an output file"
    exit(2)

infname = sys.argv[1]
outfname = sys.argv[2]
f = open(infname,'r') #input file
out = open(outfname,'w') #output file
i = 0 # to count the lines in input file
for line in f:
    i+=1
    line = line.strip() #remove the \n sign at the end of the line
    if i == 1: # no computation to do on first line
        out.write("%s,std,mean,median\n" % line)
    else:
        first_comma_index = line.index(",")
        gene_name = line[:first_comma_index] # the gene name is the first element of the line
        line = line[first_comma_index+1:] # the gene name of the line should be remove of the line to have only the numeric data
        line_data = np.fromstring(line, dtype=float, sep=',') #build a numpy array from the string
        std = np.std(line_data) #standrad deviation
        avg = np.mean(line_data) #mean
        median = np.median(line_data) #median
        out.write("%s,%s,%.5f,%.5f,%.5f\n" %(gene_name,line,std,avg,median)) #write the values in file

#close the files
f.close()
out.close()
