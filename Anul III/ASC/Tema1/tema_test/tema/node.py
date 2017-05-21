"""
	This module represents a cluster's computational node.

	Computer Systems Architecture course
	Assignment 1 - Cluster Activity Simulation
	march 2013
"""

import threading
import Queue


# Clasa necesara pentru a mentine informatiile si raspunsul despre un request

class Elem:
	def __init__(self, node_id, task_id, row, column, matrix):

		# nodul ce are nevoie de element
		self.node_id = node_id

		# indexul taskului ce a cerut elementul
		self.task_id = task_id

		self.row = row
		self.column = column
		self.matrix = matrix

	# se salveaza valoarea ceruta
	def set_value(self, value):
		self.value = value	
		

# Threadurile ce se ocupa de prelucrarea task-urilor

class Task(threading.Thread):

	def __init__(self, node):
		threading.Thread.__init__(self)
		self.node = node;

	# Cerere pentru elementul x catre datastore
	def get_element(self, elem):
		
		row = elem.row % self.node.block_size
		column = elem.column % self.node.block_size

		if elem.matrix == 0:
			return self.node.data_store.get_element_from_a(self.node, row, column)
		else:
			return self.node.data_store.get_element_from_b(self.node, row, column)

	# Salveaza elementul primit de la datastore in coada cu task_id-ul primit
	def save_element(self, elem):
		node = self.node.list_nodes[elem.node_id]
		node.queue_list[elem.task_id].put(elem)		

	# Rezolva cererile catre datastore
	def run(self):

		self.node.data_store.register_thread(self.node)

		while 1:

			# Se asteapta o noua cerere
			elem = self.node.queue.get()

			# Cerere de oprire thread
			if elem == None:
				self.node.queue.put(None)
				break

			# Se trimite cererea
			elem.set_value(self.get_element(elem))

			# Trimitere raspus catre nod
			self.save_element(elem)


class Node:
	"""
		Class that represents a cluster node with computation and storage functionalities.
	"""

	def __init__(self, node_ID, block_size, matrix_size, data_store):
		"""
			Constructor.

			@param node_ID: a pair of IDs uniquely identifying the node; 
			IDs are integers between 0 and matrix_size/block_size
			@param block_size: the size of the matrix blocks stored in this node's datastore
			@param matrix_size: the size of the matrix
			@param data_store: reference to the node's local data store
		"""

		# Salvare informatii nod
		self.node_ID = node_ID
		self.block_size = block_size
		self.matrix_size = matrix_size
		self.data_store = data_store

		# Threadurile de requesturi
		self.threads = []
		self.threads_nr = data_store.get_max_pending_requests(node_ID);

		# Coada de requesturi
		self.queue = Queue.Queue()

		# Coada in care se salveaza valorile primite
		self.queue_list = []

		# Index cerere compute matrix
		self.task_id = -1

		self.lock = threading.Lock()

		# pornire threaduri de cereri
		for i in xrange(self.threads_nr):
			thr = Task(self)
			self.threads.append(thr)
			thr.start()


	def set_nodes(self, nodes):
		"""
			Informs the current node of the other nodes in the cluster. 
			Guaranteed to be called before the first call to compute_matrix_block.

			@param nodes: a list containing all the nodes in the cluster
		"""
		self.list_nodes = {}
		for node in nodes:
			self.list_nodes[node.node_ID] = node;


	def compute_matrix_block(self, start_row, start_column, num_rows, num_columns):
		"""
			Computes a given block of the result matrix.
			The method invoked by FEP nodes.

			@param start_row: the index of the first row in the block
			@param start_column: the index of the first column in the block
			@param num_rows: number of rows in the block
			@param num_columns: number of columns in the block

			@return: the block of the result matrix encoded as a row-order list of lists of integers
		"""

		# se face cerere pentru un nou task_id
		with self.lock:
			task_id = self.task_id
			self.task_id += 1
			self.queue_list.insert(task_id, Queue.Queue())

		# se cer toate elementele de pe linii necesare din A
		for i in xrange(num_rows):
			for k in xrange(self.matrix_size):
				self.request_value(task_id, start_row + i, k, 0)

		# se cer toate elementele de pe coloanele necesare din B
		for i in xrange(num_columns):
			for k in xrange(self.matrix_size):
				self.request_value(task_id, k, start_column + i, 1)

		# se construiesc matricile in care se vor salva elementele primite
		matrixA = [ [ 0 for _ in xrange(self.matrix_size) ] for _ in xrange(num_rows) ]
		matrixB = [ [ 0 for _ in xrange(self.matrix_size) ] for _ in xrange(num_columns) ]
		matrixR = [ [ 0 for _ in xrange(num_columns) ] for _ in xrange(num_rows) ]

		# numarul de elemente cerute
		elem_to_wait = self.matrix_size * (num_rows + num_columns)

		# se asteapta primirea fiecarui element cerut
		for _ in xrange(elem_to_wait):
			elem = self.queue_list[task_id].get()

			# se salveaza in matricile corespunzatoare
			if elem.matrix == 0:
				matrixA[elem.row - start_row][elem.column] = elem.value
			else:
				matrixB[elem.column - start_column][elem.row] = elem.value

		# se calculeaza matricea rezultanta prin inmultirea A * B
		for i in xrange(num_rows):
			for j in xrange(num_columns):
				for k in xrange(self.matrix_size):
					matrixR[i][j] += matrixA[i][k] * matrixB[j][k]

		# se intoarce blocul calculat
		return matrixR

	# se ocupa de trimiterea requesturilor catre nodurile corespunzatoare
	def request_value(self, task_id, row, column, matrix):

		# se determina nodul la care trebuie trimisa ceererea
		req_node_ID = (row / self.block_size, column / self.block_size)

		# se creaza un obiect cu informatiile despre cerere
		elem = Elem(self.node_ID, task_id, row, column, matrix)

		# se trimite cererea nodului crespunzator
		self.list_nodes[req_node_ID].queue.put(elem);


	def shutdown(self):
		"""
			Instructs the node to shutdown (terminate all threads).
		"""

		# se inchid toate threadurile
		self.queue.put(None)
		for thr in self.threads:
			thr.join()	
