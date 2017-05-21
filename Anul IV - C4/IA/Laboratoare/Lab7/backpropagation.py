#!/usr/bin/python

import random

type = []
V = []

A = 5
B = 3
O = 1
miu = 0.3


MIN = [3, 68, 46, 1613, 8]
MAX = [8, 455, 230, 5140, 24.8]

def read_file(file):
	v = []
	type = []
	with open(file, 'rb') as f:
		for line in f:
			v.append(line.strip().split(","))
		
	type = v[0]
	v = v[1:301]
	
	# normalize
	for i in xrange(300):
		for j in xrange(5):
			v[i][j] = (float(v[i][j]) - MIN[j]) / (MAX[j] - MIN[j])
		v[i][7] = (float(v[i][7]) - 9) / 37.6
	
	return (type, v)

def rand_vect(size):
	return [random.uniform(0, 1) for i in xrange(size)]

def train():
	
	global wA, wB;
	
	for set in train_set:
		#########################################
		# forward propagation

		y = [0] * B

		# hidden layer output
		for i in xrange(B):
			for j in xrange(A):
				y[i]+= wA[j][i] * float(set[j])
			
			y[i]-= wA[0][-1]
				
			if (y[i] >= 0):
				y[i] = 1
			else:
				y[i] = 0
				
		# final output
		out = 0
		for i in xrange(B):
			out += wB[i][0] * y[i]

		# delta output		
		deltaOutput = float(set[7]) - out

		# delta hidden		
		# deltaH = suma Who * deltaOutput
		deltaHidden = [0] * B
		for i in xrange(B):
			deltaHidden[i] = wB[i][0] * deltaOutput

		#########################################
		# back propagation
		
		#compute new weight
		# W = W + miu * delta
		for i in xrange(B):
			wB[i][0] += miu * deltaOutput * out

		for i in xrange(A):
			for j in xrange(B):
				wA[i][j] += miu * deltaHidden[j] * y[j]

		# DEBUG
		#print
		#print "Y:", str(y)
		#print "SET:", set
		#print "OUT: ", out
		#print "DELTA OUTPUT: ", deltaOutput
		#print "DELTA HIDDEN", str(deltaHidden)
		#print "WEIGHT HIDDEN", str(wB)		
		#print "WEIGHT INPUT", str(wA)



def test():

	set_id = 0

	for set in test_set:
		#########################################
		# forward propagation

		set_id += 1
		y = [0] * B

		# hidden layer output
		for i in xrange(B):
			for j in xrange(A):
				y[i]+= wA[j][i] * float(set[j])
			
			y[i]-= wA[0][-1]
				
			if (y[i] >= 0):
				y[i] = 1
			else:
				y[i] = 0
				
		# final output
		out = 0
		for i in xrange(B):
			out += wB[i][0] * y[i]

		print "SET:", set_id, "OUTPUT:", out, "EXPECTED", set[7]


if __name__ == "__main__":
	(type, V) = read_file('MPG.csv')
	
	train_set = V[0:50]
	test_set = V[100:300]
	
	# wA = [[] * (B + 1) for i in xrange(A)]
	wA = []
	wB = []
	for i in xrange(A):
		wA.append(rand_vect(B))
		wA[i].append(1)

	for i in xrange(B):
		wB.append(rand_vect(O))
		wB[i].append(1)	

	print wA
	print wB

	for _ in xrange(1000):
		train()
	test()
