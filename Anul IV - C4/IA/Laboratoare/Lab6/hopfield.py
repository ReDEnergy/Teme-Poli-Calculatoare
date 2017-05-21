#!/usr/bin/python

from random import random
import numpy as np

WIDTH = 12                      # The width of a pattern
HEIGHT = 10                     # The height of a pattern
N = WIDTH * HEIGHT;             # The number of cells in a pattern
ALL = 10                        # The number of all different patterns

VALUE_OF = {'_' : -1, 'x' : 1}
CHAR = {-1 : '_', 1 : 'x'}

def comp(list1, list2):
    for val in list1:
        if val in list2:
            return True
    return False

def read_digits(digits_no):
	"""Returns the first digits_no patterns from file 'digits'
	as 120 character long lists of 1 and -1"""
	digits_no = min(digits_no, ALL)
	patterns = []
	with open('digits', 'r') as f:
		for d in range(digits_no):
			p = [map(lambda c : VALUE_OF[c], f.readline().rstrip())
				for i in range(HEIGHT)]
			patterns.append(reduce(lambda l1, l2 : l1 + l2, p))
			f.readline()
	return patterns

def print_digit(digit):
	"""Prints a digit (a 120 character long list of -1 and 1)"""
	for l in range(HEIGHT):
		print ''.join(map(lambda x : CHAR[x], digit[l*WIDTH:(l+1)*WIDTH]))

def add_noise(digit, noise):
	"""Adds noise to a pattern"""
	return map(lambda x : -x if random() < noise else x, digit)

def compute_weights(patterns):
	"""Computes the weights of a Hopfield network given a set of patterns"""
	
	weights = np.zeros((N, N))
	mI = np.eye(N) * len(patterns)
	weights = np.add(weights, -mI);
	
	for pattern in patterns:
		s = np.array(pattern).reshape(N, 1)
		sT = np.array(pattern).reshape(1, N)
		weights += np.multiply(sT, s)
		
	print weights
	return weights
		
def converge(weights, pattern):
	"""Recovers a learned pattern from a given one [with noise]"""
	
	print_digit(pattern);
	
	x = pattern[:]
	s = pattern[:]
	su = 0
	done = True
	while done:
		for i in xrange(N):
			su = 0;
			for j in xrange(N):
				su +=  pattern[j] * weights[i][j]
			x[i] = 1 if su > 0 else -1
		
		print x
		print s
		if s == x:
			break
		s = x[:]
		
	print_digit(s);
		

def compute_accuracy(patterns, noise):
	"""Computes the accuracy of a Hopfield network
	for a given set of patterns and a noise coefficient"""
	
	pass

if __name__ == "__main__":
	import sys
	if len(sys.argv) > 1:
		digits_no = int(sys.argv[1])
	else:
		digits_no = 3

	digits = read_digits(digits_no)
	weights = compute_weights(digits)
	noise_pattern = add_noise(digits[1], 0.3)
	converge(weights, noise_pattern)
	
	
	# print_digit(add_noise(digits[2], 0.1))
