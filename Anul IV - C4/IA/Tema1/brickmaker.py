import socket
import random
import sys
import json

brick_id = {'A':0, 'B':1, 'C':2, 'D':3, 'E':4, 'F':5, 'G':6}
Q = { 0 : 0}
D = {'A':0, 'B':0, 'C':0, 'D':0, 'E':0, 'F':0, 'G':0}

ALPHA = 0.5
GAMMA = 0.7
EPSILON = 0.1
MIN_EPSILON = 0.0001
LIMIT_EXPLORE = 0.01
STEPS = 1000

class BrickMaker:
	def __init__(self):
		self.socket = socket.socket(socket.AF_UNIX, socket.SOCK_SEQPACKET)
		self.socket.connect("s_server")
		self.send_msg("BRICKMAKER\n")
		self.buffer = b''
		self.score = 0
		
		firstLine = self.receive_msg()
		[self.height, self.length] = map(lambda x: int(x), firstLine.split(','))
		print "H: %d L: %d" % (self.height, self.length)

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

		# print str(hash)

		return key
		
	# Computes Q_KEY for (game_state/brick)
	def compute_Q_key(self, key_state, brick):
		# print "compute_Q_key", key_state, brick
		key = key_state << 3
		key |= brick_id[brick]
		return key

	# Computes the best possible action (rotation, offset) and also Q_KEY
	def get_best_action(self, state_key):
		best_brick = -1
		best_key = -1;
		max = -1000
		
		for brick in xrange(len(bricks)):
			# print "BRICK", brick, bricks[brick]
			key = self.compute_Q_key(state_key, bricks[brick])
			value = Q.setdefault(key, 0)
			if value > max:
				max = value
				best_brick = brick
				best_key = key
		
		assert best_brick != -1
		assert best_key != -1
		
		# print bricks[best_brick], best_key
		return (bricks[best_brick], best_key)

	def e_greedy(self, state_key):
		global Q
		p = random.random()

		if p < EPSILON:
			brick = random.choice(bricks)
			key = self.compute_Q_key(state_key, brick)
			value = Q.setdefault(key, 0)
		else:
			(brick, key) = self.get_best_action(state_key)

		return (brick, key)
		
	def play(self):
		global Q
		global D
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
			if parts[1] != "GAME OVER":
			
				# Get game state
				state = parts[1].split('|')
				state_key = self.get_state_key(state)
				
				###############################################
				## SARSA

				(brick, c_key) = self.e_greedy(state_key)
				Q[l_key] += ALPHA * (reward + GAMMA * Q[c_key] - Q[l_key])
				l_key = c_key;

				## SARSA
				###############################################
				D[brick] += 1
				
				# send msg
				msg = str(brick) + "\n"
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
					print "D:", D
					print "----------------------------"

				## SARSA GAME OVER
				# Q[l_key] += ALPHA * (reward - Q[l_key])
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

	bricks_list = [['A', 'B', 'C'], ['A', 'D', 'E', 'F']]

	if len(sys.argv) == 2:
		brick_dist = int(sys.argv[1])
		bricks = bricks_list[brick_dist]
	else:
		bricks = bricks_list[0]	

	print "Bricks", bricks

	#f = open('debug.txt', 'w')
	#QFILE = 'q.json'
	#Q = loadq(QFILE)
		
	brickmaker = BrickMaker()
	brickmaker.play()

	#dumpq(QFILE, Q)
	print "QSIZE:", len(Q)
