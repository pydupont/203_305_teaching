#!/usr/bin/env python
from numpy.random import normal
from random import randint

f = "gene_expression.csv"
experiences = 25
genes = 500

with open(f,'w') as f:
    for i in range(1,genes+1):
        if i == 1: f.write(",%s\n" % ",".join(map(lambda x : "experience %s" %x, range(1,experiences+1))))
        else:
            seed = randint(10,2000)
            f.write("gene %s,%s\n" % (i,",".join(map(lambda x : "%.5f" % abs(seed+normal(loc=0, scale=(seed*0.2))), range(1, experiences+1)))))
