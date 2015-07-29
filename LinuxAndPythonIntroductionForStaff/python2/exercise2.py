#!/usr/bin/env python
import os
import sys

#Exercise 2
def read_sequence_and_compute_stats(seq):
    a = seq.count("A") #Number of A
    t = seq.count("T") #Number of T
    g = seq.count("G") #Number of G
    c = seq.count("C" )#Number of C
    gc = (g+c)/float(len(seq))*10 # G+C percent
    return "%s\t%s\t%s\t%s\t%.2f" %(a,t,g,c,gc)
    


def read_file_and_find_sequences(fasta, result_file):
    if not os.path.isfile(fasta): 
        print "Error: impossible to find the fasta file"
        exit(2) # exits the scripts and returns the error code 2
    fas = open(fasta,'r')
    res = open(result_file,'w')
    res.write("Sequence\tA\tT\tG\tC\tGC\n") # header of the file
    title = ""
    sequence = ""
    for line in fas:
        line = line.strip() #remove \n at the end of the line
        if line.startswith(">"): # it is a title
            if len(title) != 0 and len(sequence) != 0: # there is allready one sequence read
                stats = read_sequence_and_compute_stats(sequence) # compute the "stats" of the sequence
                res.write("%s\t%s\n" %(title, stats)) # write the result in the result file
            title = line[1:] # we don't want the ">" sign at the beginning of the title
            sequence = ""
        sequence += line


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print "Error: the this command should have two parameters: the fasta file path and the result file path"
    else:
        read_file_and_find_sequences(sys.argv[1],sys.argv[2])
