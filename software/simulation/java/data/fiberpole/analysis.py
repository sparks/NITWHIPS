import math
import glob
import os
from pylab import *
from numpy.fft import *

data_file = open(glob.glob("mode_1_large_amp.txt")[0], 'r')

data = []

for i, line in enumerate(data_file):
	value = line.split("	")
	value = value[0:-1]
	value = [int(v) for v in value]
	data.append(value)

data = array(data)

freq = fft(data[:,1])

print freq

out_file = open("out", 'w')
for f in freq:
	print f
	out_file.write("%e\t%e\n" % (f[0], f[1]));
