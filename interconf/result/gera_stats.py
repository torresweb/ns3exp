#!/usr/bin/python

import fileinput
import sys
import numpy as np
from scipy import stats

data = []
alfa = 0.95

for line in fileinput.input(sys.argv[1:]):
    x, y = line.split()
    data = np.append(data, float(y))

cmean  = np.mean(data)
csigma = np.std(data)
interval = stats.norm.interval(alfa, loc=cmean, scale=csigma)
a = interval[0]
b = interval[1]
print cmean, csigma, a, b