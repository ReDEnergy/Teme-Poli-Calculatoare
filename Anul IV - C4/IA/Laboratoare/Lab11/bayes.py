import random
import math
import numpy as np
import sys

if __name__ == "__main__":
	
	D = [[[0, 0, 0], [0, 0, 0], [0, 0, 0]], 
		[[0, 0, 0], [0, 0, 0]], 
		[[0, 0, 0], [0, 0, 0]], 
		[[0, 0, 0], [0, 0, 0]]]
	
	v = []
	nrc = [0, 0, 0]
	
	with open("lenses.txt") as f:
		for line in f:
			list = filter(None, line.strip().split(" ")[1:])
			list = [int(x) for x in list]
			v.append(list)
			
			c = list[-1] - 1
   			nrc[c] += 1
			for i in xrange(4):
   				D[i][list[i] - 1][c] += 1

	print v
	print D
	print nrc
	
	x = [2, 0, 1, 1]
	vp = [3, 2, 2, 2]
	
	for i in xrange(3):
		P = []
		P.append(float(nrc[i]) / sum(nrc))
		for j in xrange(4):
 			P.append((float(D[j][x[j]][i]) + 1) / (nrc[i] + vp[j]))
# 			P.append(float(D[j][x[j]][i]) / nrc[i])
		
		print P
		r = 1
		for a in P:
			r *= a
			
		print r