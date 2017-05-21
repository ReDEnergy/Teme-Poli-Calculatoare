#!/usr/bin/python

import random

type = []
V = []
population = []

nodes = 10
N = 6
G = 6
delta = 100
obj = [(3, 7), (3, 4), (1, 2), (1, 9), (2, 4), (1, 5)]

def rand_vect(size):
	return [random.uniform(0, 1) for i in xrange(size)]

def fitness(node):
	f = 0;
	g = sum(node[i] * obj[i][0] for i in xrange(N))
	v = sum(node[i] * obj[i][1] for i in xrange(N))
	
	if g > G:
		v -= (g - G) * delta
		
	# print "Greutate:", s
	# print "Valuare:", f
	# print "FITNESS:", f
	return v

def random_mask(size):
	return [random.randint(0, 1) for i in xrange(size)]

def random_crossover(node_a, node_b):
	mask = random_mask(N)
	# print 'A', node_a
	# print 'B', node_b
	# print 'M', mask
	
	node = [(1 - mask[i]) * node_a[i] for i in xrange(N)]
	node = [node[i] + mask[i] * node_b[i] for i in xrange(N)]
	
	# print 'N', node
	return node

def mutation(node):
	chance = random.random()
	if chance > 0.9:
		k = random.randint(0, N - 1)
		node[k] = 1 - node[k] 
	return node

def population_info(population):
	f = [fitness(population[i]) for i in xrange(nodes)]
	info = []
	for i in xrange(nodes):
		info.append((population[i], f[i]))
	
	print info


def run(population):
	# tournament
	k = random.randint(2, nodes - 1)
	p = random.sample(population, k)
	f = [fitness(p[i]) for i in xrange(k)]
	s = []
	for i in xrange(k):
		s.append((p[i], f[i]))
		
	s = sorted(s, key = lambda node: -node[1])

	# crossover
	off1 = random_crossover(s[0][0], s[1][0])
	off2 = random_crossover(s[1][0], s[0][0])
	
	#mutation
	off1 = mutation(off1)
	off2 = mutation(off2)
	
	return (off1, off2)

if __name__ == "__main__":

	print str(obj)

	for i in xrange(nodes):
		population.append(random_mask(N))
	print str(population)
	
	for _ in xrange(10): 
		new_population = []
		for _ in xrange(nodes/2): 
			(off1, off2) = run(population)
			new_population.append(off1)
			new_population.append(off2)
		population_info(new_population)
		population = new_population 		
		
		
		
