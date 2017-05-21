import Tkinter
import random
import math
import json

width = 400
height = 400
no_pred = 0
no_traps = 0
game = 0
sin = []
cos = []
ABS_MIN = -1000000
ABS_MAX = 1000000

# Game Entity
class Entity(object):
	def __init__(self):
		self.pos = [0., 0.]
		self.spawn()

	def spawn(self):
		self.pos[0] = random.random() * width
		self.pos[1] = random.random() * height

	def dist2_to(self, E):
		delta_x = self.pos[0] - E.pos[0]
		delta_y = self.pos[1] - E.pos[1]
		return  delta_x **2 + delta_y ** 2
	
	def get_delta(self, E):
		delta_x = self.pos[0] - E.pos[0]
		delta_y = self.pos[1] - E.pos[1]
		return (int(delta_x), int(delta_y))
	
	def get_state(self, E):
		delta_x = self.pos[0] - E.pos[0]
		delta_y = self.pos[1] - E.pos[1]

		# discretization
		dist2 = int(delta_x **2 + delta_y ** 2)
		
		if dist2 >= Prey.sense2:
			return (-1, -1)
		
		angle = int(100 * (math.atan2(delta_y, delta_x) + math.pi))
		return (dist2, angle)

	
	def collides_with(self, E):
		if (self == E):
			return False
		dist_2 = self.dist2_to(E)
		colide_dist_2 = (self.size + E.size) ** 2
		return dist_2 < colide_dist_2

# Animal Class
class Animal(Entity):
	def __init__(self):
		super(Animal, self).__init__()
	
	def is_valid_move(self, dx, dy):
		dx += self.pos[0]
		dy += self.pos[1]
		if (dx < 0 or dx > width or
			dy < 0 or dy > height):
			return False
		return True
	
	def update_pos(self, dx, dy):
		self.pos[0] += dx
		self.pos[1] += dy
		if (self.pos[0] < 0 or self.pos[0] > width or
			self.pos[1] < 0 or self.pos[1] > height):
			self.pos[0] -= dx
			self.pos[1] -= dy
			return False
		return True
		
# Trap Class			
class Trap(Entity):
	size = 0
	
	def __init__(self, id):
		super(Trap, self).__init__()
		self.id = id
		
	def draw(self, sim):
		sim.draw_circle(self.pos, 2 * Trap.size, outline='#96E', fill='#F54')
		
# Prey Class			
class Prey(Animal):
	size = 0
	learning = False

	def __init__(self):
		super(Prey, self).__init__()

	def draw(self, sim):
		sim.draw_circle(self.pos, 2 * Prey.sense, outline='#0F2')
		sim.draw_circle(self.pos, 2 * Prey.size, outline='#02F', fill='#02F')
		
	def explore(self):
		while True: 
			deg = int(random.random() * 360)
			dx = sin[deg] * Prey.speed
			dy = cos[deg] * Prey.speed
			if self.update_pos(dx, dy):	
				break
	
	def get_action(self, angle):
		dx = sin[angle] * Prey.speed
		dy = cos[angle] * Prey.speed
		return (dx, dy) 

# Predator Class
class Predator(Animal):
	size = 0

	def __init__(self, id):
		super(Predator, self).__init__()
		self.attack_dist = -1;
		self.speed_factor = 1
		self.attack_group = []
		self.id = id
		self.follow = 0
		
	def set_speed_factor(self, distance):
		self.speed_factor = float(self.attack_dist) / distance
		
	def explore(self):
		if self.follow:
			delta_x = self.pos[0] - self.follow.pos[0]
			delta_y = self.pos[1] - self.follow.pos[1]
			mod = math.sqrt(delta_x ** 2 + delta_y ** 2)
			if (mod > 4 * self.size):
				dx = (delta_x / mod) * Predator.speed
				dy = (delta_y / mod) * Predator.speed
				self.update_pos(-dx, -dy)
		else:
			while True: 
				deg = int(random.random() * 360)
				dx = sin[deg] * Predator.speed
				dy = cos[deg] * Predator.speed
				if self.update_pos(-dx, -dy) is False:
					continue
			
	 			# test for collision with the other predators
	  			for predator in game.PP:
	  				if (self.collides_with(predator)):
	  					self.update_pos(-dx, -dy)
	  					continue
				break
	
	def attack_prey(self):
		delta_x = self.pos[0] - game.P.pos[0]
		delta_y = self.pos[1] - game.P.pos[1]
		mod = math.sqrt(delta_x ** 2 + delta_y ** 2)
		dx = (delta_x / mod) * Predator.speed * self.speed_factor
		dy = (delta_y / mod) * Predator.speed * self.speed_factor
		self.update_pos(-dx, -dy)

		# test for collision with the other predators
		for predator in self.attack_group:
			if self != predator:
				dpx = self.pos[0] - predator.pos[0]
				dpy = self.pos[1] - predator.pos[1]
				dmod = math.sqrt(dpx ** 2 + dpy ** 2)
				delta = 4 * (self.size - dmod); 
				if (delta > 0):
					self.update_pos(dpx * delta, dpx * delta)					
		

	def look_for_prey(self):
		self.attack_dist = self.dist2_to(game.P)
		if self.attack_dist > Predator.sense2:
			self.attack_dist = -1
		return self.attack_dist

	def update(self):
		if self.attack_dist >= 0:
			self.attack_prey()
		else:
			self.explore()
			
		self.attack_group = []
		self.follow = 0
		self.speed_factor = 1
			

	def draw(self, sim):
		sim.draw_circle(self.pos, 2 * Predator.sense, outline='#FBB')
		sim.draw_circle(self.pos, 2 * Predator.size, outline='black', fill='black')

# Game Class
class Game:
	games = 0
	wins = 0
	simulation = 0
	test_initial_collisions = False 
	msg = ["WIN", "GAME OVER - killed by trap", "GAME OVER - killed by predator"]
	
	def __init__(self):
		self.new()
		
	@staticmethod
	def set_state_sectors(sectors):
		Game.blank_state = (tuple([0] * (sectors+1)), tuple([0] * (sectors+1)))
		Game.sectors = sectors
		Game.sect_scale = 0.625 * (sectors / 4)
	
	@staticmethod
	def log():
		if QL.logging and (Game.games % 100) == 0:
			wins = float(Game.wins) / Game.games * 100
			print "WINS: ", wins, "%" , "GAMES:", Game.games
			print "ACTIONS", QL.actions
			LOG.write("games played:\t" + str(Game.games) + "\twins: " + str(wins) + '\n')
			dumpq(QL.Q, QF)

	def new(self):
		self.P = Prey()
		self.PP = [Predator(i) for i in xrange(no_pred)]
		self.TR = [Trap(i) for i in xrange(no_traps)]
		Game.test_initial_collisions = True
		Game.games +=1
		Game.log()
	
	def get_state(self):
		pstate = [0] * Game.sectors
		for predator in game.PP:
			(dx, dy) = self.P.get_delta(predator)
			dist2 = dx**2 + dy**2
			if dist2 <= Prey.sense2:
				deg = int((math.atan2(dy, dx) + math.pi) * Game.sect_scale)
				pstate[deg] += dx + dy
					
# 		print "P STATE", pstate

		tstate = [0] * Game.sectors
		for trap in game.TR:
			(dx, dy) = self.P.get_delta(trap)
			dist2 = dx**2 + dy**2
			if dist2 <= Prey.sense2:
				deg = int((math.atan2(dy, dx) + math.pi) * Game.sect_scale)
				tstate[deg] += dx + dy

# 		print "T STATE", tstate
		return (tuple(pstate), tuple(tstate))
					
	def sync_attackers(self):
		p = []
		group = []
		max_dist = -1
		for predator in self.PP:
			dist = predator.look_for_prey()
			if dist > 0: 
				p.append(predator)
				if dist > max_dist:
					max_dist = dist
			else:
				group.append(predator)

 		if len(p) > 1:
	 		for pred in p:
	 			pred.set_speed_factor(max_dist)
	 			pred.attack_group = p
	 	if len(p) == 1 and len(game.PP) > 1:
	 		p[0].attack_dist = -1
	 		group.append(p[0])
	 		
	 	for i, pred in enumerate(group):
	 		if (i%2 == 1):
	 			pred.follow = group[i-1]
	 		else:
	 			pred.follow = game.P

	def update(self):
		reward = 5
		self.sync_attackers()

		# move predators
		for predator in self.PP:
			predator.update()

		# test collisions with traps
		for trap in self.TR:
			if (trap.collides_with(self.P)):
# 				print Game.msg[1]
				state = self.get_state()
				self.new()
				return (state, -10)

		# test collisions with preadtors
		victim = 0
		for predator in self.PP:
			if (predator.collides_with(self.P)):
				if victim:
# 					print Game.msg[2]
					state = self.get_state()
					self.new()
					return (state, -10)
				victim = predator
			
			for trap in self.TR:
				if (predator.collides_with(trap)):
					if victim == predator:
						victim = 0
					self.PP.remove(predator)
					reward += 3
					if (len(self.PP) == 0):
# 						print Game.msg[0]
						Game.wins +=1
						state = self.get_state()
						self.new()
						return (state, 20)
						
					self.TR.remove(trap)
					break

		if victim:
			if (len(self.PP) <= 1):
# 				print Game.msg[0]
				Game.wins +=1
				state = self.get_state()
				self.new()
				return (state, 20)
			self.PP.remove(victim)
			reward += 5

		if Game.test_initial_collisions:
			Game.test_initial_collisions = False
			if len(self.PP) != no_pred or \
				len(self.TR) != no_traps:
				Game.games -=1
				self.new()
				return (Game.blank_state, 0)
				

		return (self.get_state(), reward)

	def simulate(self, ticks):
		time = 0
		states = set()
		while True:
			self.P.explore()
			(state, r) = self.update()
			states.add(state)
			time += 1
			if (time % 50000) == 0:
				print time
				print 'STATES', len(states)
				print "AVG WINS: ", float(Game.wins * 100) / Game.games, "%"
			if (time == ticks):
				Game.wins = 0
				Game.games = 0
				return states

	def view(self):
		Game.simulation = Simulation()
		self.view_simulation()
		Game.simulation.start()

	def view_simulation(self):
		Game.simulation.clear()
		self.P.explore()
		game.update()
		game.draw(Game.simulation)
		Game.simulation.next_frame(self.view_simulation)

	def draw(self, sim):
		for trap in self.TR:
			trap.draw(sim)
			
		for predator in self.PP:
			predator.draw(sim)
			
		self.P.draw(sim) 

# Class Kmeans			
class Kmeans:
	@staticmethod
	def dist(state_a, state_b):
		p = sum([(a-b)**2 for a, b in zip(state_a[0], state_b[0])])
		t = sum([(a-b)**2 for a, b in zip(state_a[1], state_b[1])])
# 		if p == 0 and t == 0:
# 			return ABS_MIN
		return p + t
	
	@staticmethod
	def add_states(state_a, state_b):
		p = [a + b for a, b in zip(state_a[0], state_b[0])]
		t = [a + b for a, b in zip(state_a[1], state_b[1])]
		return [p, t]

	@staticmethod
	def divide_state(state, d):
		p = [float(p) / d for p in state[0]]
		t = [float(t) / d for t in state[1]]
		return [p, t]

	@staticmethod
	def start(states, k):
		Kmeans.k = k
		Kmeans.centroids = random.sample(states, k)
		Kmeans.cl_size = [0] * k

		Kmeans.states = list(states)
		Kmeans.state_cl = [0] * len(states)

		state_size = len(Kmeans.states[0][0])
		iteration = 0 
		 
		while True:
			iteration += 1
			last = Kmeans.state_cl[:]   
			Kmeans.state_cl = map(Kmeans.get_cluster, Kmeans.states)
			Kmeans.centroids = [[[0] * state_size, [0] * state_size] for i in xrange(k)]
			size = [0] * k
			
			for i, state in enumerate(Kmeans.states):
				idx = Kmeans.state_cl[i]
				Kmeans.centroids[idx] = Kmeans.add_states(Kmeans.centroids[idx], state)
				size[idx] += 1  

			Kmeans.centroids = [Kmeans.divide_state(Kmeans.centroids[i], size[i]) for i in xrange(k)]
# 			print Kmeans.centroids
			
			print "ITERATION", iteration
			
			if last == Kmeans.state_cl:
				break
		
		return Kmeans.centroids
					
	@staticmethod
	def get_cluster(state):
		mins = [Kmeans.dist(state, centroid) for centroid in Kmeans.centroids]
		m = ABS_MAX
		k = 0
		for i, min in enumerate(mins):
			 if (min < m):
			 	m = min
			 	k = i
		return k

# Return stable state
class SStates:

	sstates = 0
	
	@staticmethod
	def get_stable(state):
		min = ABS_MAX
		st = -1 
		for i, stable in enumerate(SStates.sstates):
			dist = Kmeans.dist(state, stable)
			if dist < min:
				min = dist
				st = i

		assert st != -1
		return st

# Class Q-Learning
class QL:
	Q = []
	logging = False
	simulation = 0
	ALPHA = 0.7
	GAMMA = 0.7
	prev_state = 0
	prev_action = 0
	max_actions = 0
	section_angle = 0
	history = []

	@staticmethod
	def init(size):
		QL.max_actions = Game.sectors + 1
		QL.section_angle = 360 / Game.sectors
		QL.Q = [[0] * QL.max_actions for i in xrange(size)]
		QL.actions = [0] * QL.max_actions
		
	@staticmethod
	def load_Q(file):
		QL.Q = loadq(file)
	
	@staticmethod
	def learn():
		(state, reward) = game.update()
		action = QL.get_action(state, reward)
		game.P.update_pos(action[1], action[2])

		# draw
		QL.simulation.clear()
		game.draw(QL.simulation)
		QL.simulation.next_frame(QL.learn)

	@staticmethod
	def learn2():
		while True:
			(state, reward) = game.update()
			action = QL.get_action(state, reward)
			game.P.update_pos(action[1], action[2])

	@staticmethod
	def start(Debug):
		if Debug:
			QL.simulation = Simulation()
			QL.learn()
			QL.simulation.start()
		else:
			QL.learn2()
		
	@staticmethod	
	def get_action(state, reward):
		
		state = SStates.get_stable(state)

		action = (QL.max_actions-1, 0, 0)
		max = QL.Q[state][action[0]]
		
		for i in xrange(Game.sectors):
			(dx, dy) = game.P.get_action(i * QL.section_angle)
			if game.P.is_valid_move(dx, dy) is False:
				continue
			
			value = QL.Q[state][i] 	

			if value > max:
				max = value
				action = (i, dx, dy)

		QL.Q[QL.prev_state][QL.prev_action] += QL.ALPHA * (reward + QL.GAMMA * max - QL.Q[QL.prev_state][QL.prev_action]) 

		QL.prev_state = state
		QL.prev_action = action[0]
		QL.actions[action[0]] +=1

# 		assert action != -1
		return action

# Simulation Class
class Simulation:
	
	delta_time = 10		# miliseconds
	
	def __init__(self):
		self.window = Tkinter.Tk()
		self.canvas = Tkinter.Canvas(self.window, width=width, height=height)
		self.canvas.pack()
	
	def draw_circle(self, position, size, fill='', outline='black'):
		top_left_x = position[0] - size/2
		top_left_y = position[1] - size/2
		bottom_right_x = position[0] + size/2
		bottom_right_y = position[1] + size/2
		
		self.canvas.create_oval(top_left_x, top_left_y, 
								bottom_right_x, bottom_right_y,
								width="1", outline=outline, fill = fill)
	
	def clear(self):
		self.canvas.delete(Tkinter.ALL)
	
	def start(self):
		self.window.mainloop()
		
	def next_frame(self, func):
		self.window.after(Simulation.delta_time, func)
		
# Precompute sin and cos math functions
def precompute_math():
	global sin, cos
	rad = 2 * math.pi
	sin = [math.sin(float(i)/360 * rad) for i in xrange(360)]
	cos = [math.cos(float(i)/360 * rad) for i in xrange(360)]

def read_game_parameters():
	global width, height, no_pred, no_traps
	
	def read_int(file):
		return int(file.readline().split('\t')[0])

	def read_float(file):
		return float(file.readline().split('\t')[0])
	
	with open('params.txt', 'r') as f:
		width = read_int(f)
		height = read_int(f)

		Prey.speed = read_float(f)
		Prey.size  = read_int(f)
		Prey.sense =  read_int(f)		
		Prey.sense2 = Prey.sense ** 2

		no_pred = read_int(f)
		Predator.speed = read_float(f)
		Predator.size = read_int(f)
		Predator.sense = read_int(f)
		Predator.sense2 = Predator.sense ** 2
		
		no_traps  = read_int(f)
		Trap.size = read_int(f)

# Functions for writing set to a JSON file
def dumpq(S, filename):
	with open(filename, 'w') as f:
		json.dump(list(S), f)
		
# Functions for loading set from a JSON file
def loadq(filename):
	with open(filename, 'r') as f:
		q = json.load(f)
	return list(q)		
		
if __name__ == "__main__":

	QLoad = 'qlearning.txt'
	QF = 'qlearning.txt'
	KF = 'kmeans.txt'
	LOG = open('debug.txt', 'w')

	precompute_math()
	read_game_parameters()

	Game.set_state_sectors(8)
	game = Game()
	
# 	game.view()

	states = game.simulate(50000)
	dumpq(Kmeans.start(states, 512), KF)
	SStates.sstates = loadq(KF)

	# test if Loaded Q = computed Q
# 	assert len(SStates.sstates[0][0]) == len(next(iter(states))[0]) 

	QL.logging = True
 	QL.init(1024)
# 	QL.load_Q(QLoad)
	QL.start(0)
