import socket
import random
import sys
import json

max_offset = {
	'A':[-4, -1],
	'B':[-3, -2, -3, -2],
	'C':[-3, -2, -3, -2],
	'D':[-3, -2, -3, -2],
	'E':[-3, -2],
	'F':[-3, -2],
	'G':[-2]
}
brick_id = {'A':0, 'B':1, 'C':2, 'D':3, 'E':4, 'F':5, 'G':6}
rotations = {'A':2, 'B':4, 'C':4, 'D':4, 'E':2, 'F':2, 'G':1}
Q = { 0 : 0}

ALPHA = 0.2
GAMMA = 0.5
EPSILON = 0.1
MIN_EPSILON = 0.0001
LIMIT_EXPLORE = 0.01
STEPS = 1000

class BrickLayer:
	def __init__(self):
		self.socket = socket.socket(socket.AF_UNIX, socket.SOCK_SEQPACKET)
		self.socket.connect("s_server")
		self.send_msg("BRICKLAYER\n")
		self.buffer = b''
		self.score = 0
		self.nr_states = 0
		
		firstLine = self.receive_msg()
		[self.height, self.length] = map(lambda x: int(x), firstLine.split(','))
		print "H: %d L: %d" % (self.height, self.length)

	# Computes maximum offset of each brick based on it's rotation
		for offset in max_offset.itervalues():
			for i in xrange(len(offset)):
				offset[i] += self.length
				
					
	# Computes game_state key
	def get_state_key(self, state):
		hash = [0] * self.length;
		for i in xrange(1, self.height + 1):
			for j in xrange(self.length):
				if hash[j] == 0 and state[i][j] == '#':
					hash[j] = self.height - i + 1
		key = 0
		
		for i in xrange(self.length):
			key |= hash[i] << (4 * i)

		return key
		
	# Computes Q_KEY for (game_state/action)
	def compute_Q_key(self, key_state, brick, rotation, offset):
		key = key_state << 8
		key |= brick_id[brick] << 5
		key |= rotation << 3
		key |= offset
		return key

	# Computes the best possible action (rotation, offset) and also Q_KEY
	def get_best_action(self, state_key, brick):
		best_rot = -1
		best_off = -1
		best_key = -1;
		max = -1000
		
		for rot in xrange(rotations[brick]):
			for off in xrange(max_offset[brick][rot] + 1):
				key = self.compute_Q_key(state_key, brick, rot, off)
				value = Q.setdefault(key, 0)
				if value > max:
					max = value
					best_rot = rot
					best_off = off
					best_key = key
		
		assert best_rot != -1
		assert best_off != -1
		assert best_key != -1

		return (best_rot, best_off, best_key)

	def e_greedy(self, state_key, brick):
		global Q
		p = random.random()

		if p < EPSILON:
			rotation = random.randint(0, rotations[brick] - 1)
			offset = random.randint(0, max_offset[brick][rotation])
			key = self.compute_Q_key(state_key, brick, rotation, offset)
			value = Q.setdefault(key, 0)
		else:
			(rotation, offset, key) = self.get_best_action(state_key, brick)

		return (rotation, offset, key)
		
	def play(self):
		global Q
		global EPSILON
		game_nr = 0
		wins = 0
		l_key = 0
		line = self.receive_msg()
		
		while line:
			# Parse line
			parts = line.split(',')

			# Reward
			reward = int(parts[0])
			self.score += reward

			# NOT GAME OVER
			if len(parts) == 3:
			
				# Get game state
				state = parts[1].split('|')
				state_key = self.get_state_key(state)

				# Get next brick
				brick = parts[2]

				###############################################
				## SARSA

				(rotation, offset, c_key) = self.e_greedy(state_key, brick)
				Q[l_key] += ALPHA * (reward + GAMMA * Q[c_key] - Q[l_key])
				l_key = c_key;

				## SARSA
				###############################################
				
				# send msg
				msg = str(rotation) + "," + str(offset) + "\n"
				self.send_msg(msg)
			else:
				game_nr += 1
				if self.score > 0:
					wins+=1;

				self.score = 0
				
				if game_nr % STEPS == 0:
					EPSILON = max(MIN_EPSILON, EPSILON - LIMIT_EXPLORE)
					print "GAME number %d finished" % (game_nr)
					print "EPSILON", EPSILON
					print "WINS %d PERCENTAGE %.2f%%" % (wins, 100 * float(wins) / game_nr)
					print "----------------------------"

				## SARSA GAME OVER
				Q[l_key] += ALPHA * (reward - Q[l_key])
				## SARSA

			line = self.receive_msg()

	# Print readable game state to file		
	def print_state(self, state):
		for i in xrange(1, self.height + 1):
			f.write("|" + state[i] + "|\n")
		f.write("--------\n")
	
	# Send a message to the server
	def send_msg(self, msg):
		totalsent = 0
		while totalsent < len(msg):
			sent = self.socket.send(msg[totalsent:])
			if sent == 0:
				raise RuntimeError("SOCKETUL ERROR")
			totalsent = totalsent + sent

	# Receive message from the server
	def receive_msg(self):
		while '\n' not in self.buffer:
			chunk = self.socket.recv(1024)
			if chunk == b'':
				return False
			self.buffer = self.buffer + chunk
		line = self.buffer[0:self.buffer.index('\n')]
		self.buffer = self.buffer[self.buffer.index('\n')+1:]
		return line

# Functions for loading Q from a JSON file
def loadq(fname):
	with open(fname) as f:
		q = json.load(f)
	return dict(q)
	
# Functions for writing Q to a JSON file
def dumpq(fname, q):
	with open(fname, 'w') as f:
		json.dump(q.items(), f)

if __name__ == "__main__":
	#f = open('debug.txt', 'w')
	
	#QFILE = 'q.json'
	#Q = loadq(QFILE)
		
	bricklayer = BrickLayer()
	bricklayer.play()

	#dumpq(QFILE, Q)
	print "QSIZE:", len(Q)