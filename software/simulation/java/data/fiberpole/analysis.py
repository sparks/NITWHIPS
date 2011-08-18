import math
import glob
import os
from pylab import *
from numpy.fft import *

def basicFFT(file_name):
	data_file = open(glob.glob(file_name)[0], 'r')

	data = []

	for i, line in enumerate(data_file):
		value = line.split("	")
		value = value[0:-1]
		value = [int(v) for v in value]
		data.append(value)

	data = array(data)

	freqX = fft(data[:,1])
	freqY = fft(data[:,2])
	freqZ = fft(data[:,3])
	
	magX = [abs(f) for f in freqX]
	magY = [abs(f) for f in freqY]
	magZ = [abs(f) for f in freqZ]

	plot(magX)
	plot(magY)
	plot(magZ)

	show()
	
	#print freq

	#out_file = open("out", 'w')
	#for f in freq:
	#	out_file.write("%e\t%e\n" % (abs(f), angle(f)));

basicFFT("base_line.txt")

