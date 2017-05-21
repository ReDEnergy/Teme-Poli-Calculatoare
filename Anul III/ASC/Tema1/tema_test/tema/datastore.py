"""
    This module provides the storage part of the cluster's nodes.    

    Computer Systems Architecture course
    Assignment 1 - Cluster Activity Simulation
    march 2013
"""


from random import random
from threading import current_thread, Lock, Semaphore
from time import sleep


class DataStore:
    """
        Class that represents the storage functionalities of a node.
    """

    def __init__(self, matrix_a, matrix_b, max_pending_requests, min_delay, max_delay, checker):
        self.matrix_a = matrix_a
        self.matrix_b = matrix_b
        self.max_pending_requests = max_pending_requests
        self.min_delay = min_delay
        self.max_delay = max_delay
        self.checker = checker
        self.remaining_requests = Semaphore(max_pending_requests)
        self.node = None
        self.node_threads = set()
        self.node_lock = Lock()


    def __check_access(self, node):
        with self.node_lock:
            if self.node == None:
                #ERROR: no node registered
                self.checker.report("node " + str(node) + " is accessing unregistered datastore")
                return

            if node != self.node:
                #ERROR: called by wrong node
                self.checker.report("node " + str(node) + 
                                    " is accessing datastore registered by node " + str(self.node))
                return

            if current_thread() not in self.node_threads:
                #ERROR: called from wrong thread
                self.checker.report("thread " + str(current_thread()) + 
                                    " owned by node " + str(node) + 
                                    " is not registered with datastore owned by node " + str(self.node))
                return


    def __get_element(self, node, row, column, matrix):
        self.__check_access(node)

        if not self.remaining_requests.acquire(False):
            #ERROR: no more remaining requests
            self.checker.report("maximum pending datastore requests exceeded on node " + str(node))
            return
        sleep(self.min_delay + (self.max_delay - self.min_delay) * random())
        self.remaining_requests.release()

        return matrix[row][column]


    def register_thread(self, node):
        """
            Registers the current thread of the given node to the data store. The current thread is determined automatically with threading.current_thread(). This thread will be associated with the node given as an argument and registered to the datastore. The thread will then be able to query the datastore. Multiple threads (logically owned by node) can call this method and register to the datastore.

            @param node: the node owning the data store
        """
        self.checker.register_thread(node)

        with self.node_lock:
            if self.node == None:
                self.node = node

            self.node_threads.add(current_thread())

        self.__check_access(node)


    def get_max_pending_requests(self, node):
        """
            Returns the maximum number of in-flight requests supported by this data store.

            @param node: the node accessing the data store; must be the node that owns the data store

            @return: the maximum number of in-flight requests supported by this data store
        """

        return self.max_pending_requests


    def get_element_from_a(self, node, row, column):
        """
            Returns an element from the matrix A stored in the data store. 
            This is a blocking operation. The maximum number of in-flight requests is 
            limited, see get_max_pending_requests().

            @param node: the node accessing the data store; must be the node that owns the data store
            @param row: the element row
            @param column: the element column

            @return: the element of matrix A at position (row, column)
        """
        return self.__get_element(node, row, column, self.matrix_a)


    def get_element_from_b(self, node, row, column):
        """
            Returns an element from the matrix B stored in the data store. 
            This is a blocking operation. The maximum number of in-flight requests is 
            limited, see get_max_pending_requests().

            @param node: the node accessing the data store; must be the node that owns the data store
            @param row: the element row
            @param column: the element column

            @return: the element of matrix B at position (row, column)
        """
        return self.__get_element(node, row, column, self.matrix_b)
