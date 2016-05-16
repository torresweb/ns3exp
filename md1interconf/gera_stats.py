#!/usr/bin/python

import fileinput
import sys
import numpy as np
from scipy import stats
from math import sqrt

data = []
alfa = 0.95
N = 0

for line in fileinput.input(sys.argv[1]):
    x, y = line.split()
    data = np.append(data, float(y))
    N = N + 1

cmean  = np.mean(data)
csigma = np.std(data)

# se o segundo argumento for 1 divide por sqrt(N)
if (sys.argv[2] == '1'):
	csigma = csigma/sqrt(N)
	
# se o terceiro argumento for 1 faz t_student
if (sys.argv[3] == '1'):
	interval = stats.t.interval(alfa, N - 1, loc=cmean, scale=csigma)
else:
	interval = stats.norm.interval(alfa, loc=cmean, scale=csigma)

a = interval[0]
b = interval[1]
print float(x)/100.0, cmean, csigma, a, b
