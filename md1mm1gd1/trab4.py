#!/usr/bin/python

import fileinput
import subprocess
import sys
import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
from scipy import stats
from math import sqrt

# https://en.wikibooks.org/wiki/Fundamentals_of_Transportation/Queueing
def md1_curve(xlist, mu):
    ylist = []
    for x in xlist:
        ylist.append(float((x * mu)) / (2 * (1 - x)))
        
    return ylist

def mm1_curve(xlist, mu):
    # http://www.engr.uconn.edu/~lester/papers/chapt2
    # http://pages.cs.wisc.edu/~dsmyers/cs547/lecture_12_mm1_queue.pdf
    ylist = []
    for x in xlist:
        ylist.append(mu * x / (1 - x))

    return ylist

    
def run_simulator(u=90, r=1, s=0, m=0, p=1000, l=970, w=100, shape=2.0):
    # u fator de utilizacao
    # r runseed
    # s show :  0 stats, 1 interval, 2 packetsize
    # m mode: 0 md1, 1 mm1, 2 gd1
    # p amount of packtets to send
    # l lenght each packet (payload only)
    # w warmup
    # shape pareto shape
    
    opt_u = '--u='+str(u)
    opt_r = '--r='+str(r)
    opt_s = '--s='+str(s)
    opt_m = '--m='+str(m)
    opt_p = '--p='+str(p)
    opt_l = '--l='+str(l)
    opt_w = '--w='+str(w)
    opt_shape = '--shape='+str(shape)
    
    simulator = ["/home/emula/ns3/build/scratch/ns3exp/ns3exp", opt_u, opt_r, opt_s, opt_m, opt_p, opt_w, opt_shape]
    p = subprocess.Popen(simulator, stdout=subprocess.PIPE)
    output, err = p.communicate()
    data = output.split()
    return data

packets = 1100
warmup = 100
packetsize = 1000
bandwidth = 10000000 #10Mbps

runseed = 1
util = 90
shape = 2.0
color = ['r','g','b']
label = ['m/d/1', 'm/m/1', 'g/d/1']

# Histograma de intervalo entre pacotes gerados para md1
plt.figure()
show = 1
mode = 0
data = []
output = run_simulator(util, runseed, show, mode, packets, packetsize, warmup,shape)
data = [float(x) for x in output]
n, bins, patches = plt.hist(data, 50, normed=1, facecolor=color[mode], alpha=0.75)
plt.title('M/D/1 Distribuicao do intervalo entre os pacotes - 90% util.')
#plt.show()

sys.exit()

# Histograma de tamanho de pacotes gerados para mm1
plt.figure()
show = 2
mode = 1
data = []
output = run_simulator(util, runseed, show, mode, packets, packetsize, warmup, shape)
data = [int(x) for x in output]
n, bins, patches = plt.hist(data, 50, normed=1, facecolor=color[mode], alpha=0.75)
plt.title('M/M/1 Distribuicao dos tamanhos dos pacotes - 90% util.')
#plt.show()

# Histograma de intervalo entre pacotes gerados para md1
plt.figure()
show = 1
mode = 2
data = []
output = run_simulator(util, runseed, show, mode, packets, packetsize, warmup, shape)
data = [float(x) for x in output]
n, bins, patches = plt.hist(data, 50, normed=1, facecolor=color[mode], alpha=0.75)
plt.title('G/D/1 Distribuicao do intervalo entre os pacotes - 90% util.')
#plt.show()

sys.exit()

# curvas teoricas e intervalos de confianca
plt.figure()
mu = float(packetsize * 8) / bandwidth 
x = np.arange(0, 1, 0.01);
# curva teorica md1
md1y = md1_curve(x,mu)
plot1, = plt.plot(x,md1y, color=color[0])
# curva teorica mm1
mm1y = mm1_curve(x,mu)
plot2, = plt.plot(x,mm1y, color=color[1])

alfa = 0.95
N = 0
util_factor = [0.5, 0.8, 0.9, 0.95, 0.99]
# modes:
# 0 md1 = red
# 1 mm1 = green
# 2 gd1 = blue
show = 0
for mode in range(0,3):
    y = []
    y_a = []
    y_b = []
    for util in util_factor:
        util = util * 100
        data=[]
        for seed in range(7):
            output = run_simulator(util, seed, show, mode, packets, packetsize, warmup, shape)
            #print output
            data.append(float(output[1]))
        N = len(data)
        cmean  = np.mean(data)
        y.append(cmean)
        csigma = np.std(data)/sqrt(N)
        if (csigma != 0):
            a, b = stats.t.interval(alfa, N - 1, loc=cmean, scale=csigma)
            y_a.append(a)
            y_b.append(b)
        else:
            y_a.append(0.0)
            y_b.append(0.0)
            
    #print y
    #print y_a
    #print y_b
    plt.errorbar(util_factor, y, yerr=[y_a, y_b], fmt='o',ecolor=color[mode], color = color[mode], label=label[mode] )   


plt.xlim(0.0, 1.0)
plt.xticks(np.arange(0.0, 1.0, .1))
plt.legend()
plt.show()


