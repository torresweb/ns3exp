#!/usr/bin/python

import fileinput
import subprocess
import sys
import numpy as np
import matplotlib.mlab as mlab
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
from scipy import stats
from math import sqrt
from matplotlib.backends.backend_pdf import PdfPages


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
    # l lenght each packet
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
    
    simulator = ["/home/emula/ns3/build/scratch/ns3exp/ns3exp", opt_u, opt_r, opt_s, opt_m, opt_p, opt_l, opt_w, opt_shape]
    #print simulator
    p = subprocess.Popen(simulator, stdout=subprocess.PIPE)
    output, err = p.communicate()
    data = output.split()
    return data

packets =  110000
warmup =    10000
packetsize = 1000
bandwidth = 10000000 #10Mbps

runseed = 1
util = 90
shape = 50.0
outputfile = 'alltogether-shape-50.pdf'

color = ['r','g','b']
label = ['m/d/1', 'm/m/1', 'g/d/1']

pp = PdfPages(outputfile)

together = 10

# Histograma de intervalo entre pacotes gerados para md1
show = 1
mode = 0
data = []
output = run_simulator(util, runseed, show, mode, packets, packetsize, warmup, shape)
data = [float(x) for x in output]
meandata  = np.mean(data)
stddata = np.std(data)
#print "md1 ", meandata, stddata
plt.figure(11)
plt.hist(data, 50, normed=1, facecolor=color[mode], alpha=0.5)
rv = stats.expon(meandata,stddata)
x = np.linspace(rv.ppf(0.0), rv.ppf(0.99), 100)
plt.plot(x - meandata, rv.pdf(x), 'r-', lw=1, alpha=1.0, label='Expon pdf M/D/1', color=color[mode])
plt.title('M/D/1 Distribuicao do intervalo entre os pacotes - 90% util.')
plt.legend()
plt.savefig(pp, format='pdf')
# grafico junto md1/gd1
plt.figure(together)
plt.hist(data, 50, normed=1, facecolor=color[mode], alpha=0.5)
plt.plot(x - meandata, rv.pdf(x), 'r-', lw=1, alpha=1.0, label='Expon pdf #M/D/1', color=color[mode])
#plt.show()

#quit()

# Histograma de intervalo entre pacotes gerados para gd1
show = 1
mode = 2
rdata = []
output = []
output = run_simulator(util, runseed, show, mode, packets, packetsize, warmup, shape)
rdata = [float(d) for d in output]
meandata  = np.mean(rdata)
stddata = np.std(rdata)
#print "gd1 ", meandata, stddata
plt.figure(12)
plt.hist(rdata, 100, normed=1, facecolor=color[mode], alpha=0.5)

#saida = ''
#for d in output:
#    saida = saida + str(d) + ' '
#    
#print saida

#quit()

### lomax
#scale =  meandata * (shape - 1.0) / shape;
c = shape
# lomax teorema reverso
rvl = stats.lomax(c, meandata, stddata)
lx = np.linspace(rvl.ppf(0.0), rvl.ppf(0.99), 100)
plt.plot(lx - meandata, rvl.pdf(lx), 'r-', lw=1, alpha=1.0, label='Lomax pdf - G/D/1', color=color[mode])

# verificando g/d/1 com expon
rve = stats.expon(meandata, stddata)
ex = np.linspace(rve.ppf(0.0), rve.ppf(0.99), 100)
plt.plot(ex - meandata, rve.pdf(ex), 'r-', lw=1, alpha=1.0, label='Expon pdf - G/D/1', color='black')
plt.title('G/D/1 Distribuicao do intervalo entre os pacotes - 90% util.')
plt.legend()
plt.savefig(pp, format='pdf')
#plt.show()

#pp.close()
#quit()

# grafico junto md1/gd1
plt.figure(together)
plt.title('M/D/1 e G/D/1 - Distribuicao do intervalo entre os pacotes - 90% util.')
plt.hist(rdata, 100, normed=1, facecolor=color[mode], alpha=0.5)
plt.plot(lx - meandata, rvl.pdf(lx), 'r-', lw=1, alpha=1.0, label='Lomax pdf - G/D/1', color=color[mode])
plt.plot(ex - meandata, rve.pdf(ex), 'r-', lw=1, alpha=1.0, label='Expon pdf - G/D/1', color='black')
plt.legend()
plt.savefig(pp, format='pdf')

#plt.show()
#pp.close()
#quit()

# Histograma de tamanho de pacotes gerados para mm1
plt.figure(13)
show = 2
mode = 1
data = []
output = run_simulator(util, runseed, show, mode, packets, packetsize, warmup, shape)
data = [int(x) for x in output]
meandata  = np.mean(data)
#print "mm1 ", meandata
stddata = np.std(data)
plt.hist(data, 50, normed=1, facecolor=color[mode], alpha=0.5)
rv = stats.expon(meandata,stddata)
x = np.linspace(rv.ppf(0.0), rv.ppf(0.99), 100)
plt.plot(x-meandata, rv.pdf(x), 'r-', lw=1, alpha=1.0, label='Expon pdf M/M/1', color=color[mode])
plt.title('M/M/1 Distribuicao dos tamanhos dos pacotes - 90% util.')
plt.legend()
plt.savefig(pp, format='pdf')
#plt.show()


#pp.close()
#quit()

alfa = 0.95
N = 0
util_factor = [0.5, 0.8, 0.9, 0.95, 0.99]
show = 0
together = 20
plt.figure(together)
for mode in range(0,3):
    y = []
    y_a = []
    y_b = []

    if (mode == 0):
        # curvas teoricas e intervalos de confianca
        mu = float(packetsize * 8) / bandwidth
        x = np.arange(0, 1, 0.01);
        # curva teorica md1
        md1y = md1_curve(x,mu)
        plt.figure(together)
        plt.plot(x,md1y, color=color[mode])
        plt.figure(mode)
        plt.plot(x,md1y, color=color[mode])


    if (mode == 1):
        # curvas teoricas e intervalos de confianca
        mu = float(packetsize * 8) / bandwidth
        x = np.arange(0, 1, 0.01);
        # curva teorica mm1
        mm1y = mm1_curve(x,mu)
        plt.figure(together)
        plt.plot(x,mm1y, color=color[mode])
        plt.figure(mode)
        plt.plot(x,mm1y, color=color[mode])
 
 
    if (mode == 2):
        plt.figure(mode)

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
                
    plt.figure(together)
    plt.errorbar(util_factor, y, yerr=[y_a, y_b], fmt='o',ecolor=color[mode], color = color[mode], label=label[mode] )
    
    plt.figure(mode)
    plt.errorbar(util_factor, y, yerr=[y_a, y_b], fmt='o',ecolor=color[mode], color = color[mode], label=label[mode] )
    plt.legend()
    plt.xlim(0.0, 1.0)
    plt.xticks(np.arange(0.0, 1.0, .1))
    plt.savefig(pp, format='pdf')
    

plt.figure(together)
plt.legend()
plt.xlim(0.0, 1.0)
plt.xticks(np.arange(0.0, 1.0, .1))
plt.savefig(pp, format='pdf')
pp.close()
#plt.show()


