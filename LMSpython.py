# -*- LMS-*-
"""
author: parth parikh

"""

#import matplotlib.pyplot as plt
import numpy
from numpy import zeros
from pylab import *
from scipy.io import wavfile
import sys

eps = sys.float_info.epsilon

order = 64;


# Read the reference noise
Fs, ref = wavfile.read('noise3.wav')
ref = ref - mean(ref)

N = len(ref)
t = numpy.linspace(0.0,float(N)/Fs,N)


# Read the noisy speech
Fn, noisy = wavfile.read('speech3.wav')
noisy = noisy - mean(noisy)

## Read the clean speech signal
#Fc, clean = wavfile.read('sp22.wav')

# Estimate the step size
mu = 2/(order*(var(ref)))


weights = zeros(order)
desired = zeros(N)
buff    = zeros(order)

# nLMS algorithm
for i in range(0,N):
    buff[1:] = buff[:len(buff)-1]
    buff[0] = ref[i]
    desired[i] = noisy[i]-numpy.dot(buff,weights)
    weights = weights + (eps + (mu*desired[i]*buff))/(buff.dot(buff))



#err = clean[10:] - desired[10:]
#SE = (err**2)
#MSE = sum(SE)/len(SE)

# Write filtered output for signal analysis
wavfile.write('desired.wav',Fs,desired)
