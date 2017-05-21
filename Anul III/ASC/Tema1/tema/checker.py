"""
    This module is used for checking the validity and correctness of the multithreaded 
    behaviour of the simulated nodes

    Computer Systems Architecture course
    Assignment 1 - Cluster Activity Simulation
    march 2013
"""


from threading import current_thread, Lock


class Checker:
    def __init__(self):
        self.banned_threads = []
        self.banned_threads_lock = Lock()
        self.node_threads = {}
        self.node_threads_lock = Lock()
        self.messages = []
        self.messages_lock = Lock()


    def register_thread(self, node):
        with self.banned_threads_lock:
            if current_thread() in self.banned_threads:
                #ERROR: called by wrong thread
                self.report("node " + str(node) + " is trying to register tester thread")
                return

        with self.node_threads_lock:
            if node not in self.node_threads:
                self.node_threads[node] = set()

            self.node_threads[node].add(current_thread())

            for n in self.node_threads.keys():
                if n != node:
                    if current_thread() in self.node_threads[n]:
                        #ERROR thread registered with multiple nodes
                        self.report("node " + str(node) + " is trying to register thread " + 
                                    str(current_thread()) + " already registered by node " + str(n))


    def add_banned_thread(self, thread):
        with self.banned_threads_lock:
            self.banned_threads.append(thread)


    def report(self, message):
        with self.messages_lock:
            self.messages.append(message)


    def status(self):
        return self.messages
