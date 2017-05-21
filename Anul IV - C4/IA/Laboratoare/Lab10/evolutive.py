import random
import math
import numpy as np
import sys

c1 = 20
c2 = 0.2
c3 = 2 * math.pi
sigma = 0.8
c = 0.845 

def f(xuri):
	s1 = sum([x * x for x in xuri])
	s2 = sum([math.cos(c3 * x) for x in xuri])
	inv = 1.0 / len(xuri)
	
	r = -c1 * math.exp(-c2 * math.sqrt( inv * s1)) - math.exp(inv * s2) + c1 + math.e
	return r

def get_sigma(ps):
	global sigma
	if ps < 0.2:
		sigma = sigma / c
	if ps > 0.2:
		sigma = sigma * c
	return sigma

if __name__ == "__main__":

	x = [1.5, 7.8]
	succ = 0

	for i in xrange(200000):
		z = [random.gauss(0, math.pi) for i in xrange(2)]
		y = [z[i] + x[i] for i in xrange(2)]
		if (f(y) < f(x)):
			x = y
# 			succ += 1
		else:
			sigma = max( sigma + random.gauss(0, math.pi), 0.01)

# 		ps = float(succ) / (i+1)
# 		sigma = get_sigma(ps)
		
	print x, sigma, f(x)
