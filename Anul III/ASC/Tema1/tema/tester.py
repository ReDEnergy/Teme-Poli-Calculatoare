
#!/usr/bin/python

"""
    Testing infrastructure - generates and executes tests, 
    simulates a cluster's FEPs (front-end processors)

    Computer Systems Architecture course
    Assignment 1 - Cluster Activity Simulation
    march 2013


"""

import random
import sys
import os
import re
import getopt
import math
from threading import *
from checker import Checker
from node import Node
from datastore import DataStore

#DEBUG = True
DEBUG = False


class Tester:   
    """ Generates test scenarios and simulates the users and front-end-processors
        of a cluster, which send jobs to the computational and storage nodes.
    """
    def __init__(self, output_filename):
        """
            Constructor.
            @type output_filename: String
            @param output_filename: the file in which the tester logs results
        """
        self.output_filename = output_filename
        
        self.test_generator = TestGenerator()    
        
        self.print_lock = Lock()
        self.passed = Event()
        self.passed_tests = 0
        self.finished_output_lock = Lock()
        self.finished_output = False       
 
    def run_test(self, testfile = None, times = 1):
        """
            Performs a test. The test is generated from a given file or randomly.
            To better check for synchronization errors the test should be run several times,
            as given by the 'times' parameter. 
            @type testfile: String
            @param testfile: the path+name of the file containing the test parameters
            @type times: Integer
            @param times: number of time to run the test
        """
        if not testfile:
            test = self.test_generator.generate_random_test()
        else:
            test = self.test_generator.generate_test_from_file(testfile)

        t = Timer(interval=test.timeout, function=Tester.timer_fn, args=[self, test, times])
        t.start()

        for i in range(times):
            self.start_test(test)

        t.cancel()
        with self.finished_output_lock:
            if not self.finished_output:
                msg = "Test %-10s Finished.................................%d%% completed"%(test.name, 100.0*self.passed_tests/times)
                self.safe_print(msg)
                out_file = open(self.output_filename, "a")
                out_file.write(msg + "\n")
                out_file.close()
                self.finished_output = True 

    def timer_fn(self, test, num_times):
        with self.finished_output_lock:
            if not self.finished_output:
                if self.passed_tests == num_times:
                    msg = "Test %-10s Finished..................................%d%% completed"%(test.name, 100.0*self.passed_tests/num_times)
                else:
                    msg = "Test %-10s Timeout..................................%d%% completed"%(test.name, 100.0*self.passed_tests/num_times)
                self.safe_print(msg)
                out_file = open(self.output_filename, "a")
                out_file.write(msg + "\n")
                out_file.close()
                self.finished_output = True 
                os._exit(0)

    def start_test(self, test):
        """
            Starts the execution of a given test. It generates the nodes and creates threads that 
            send tasks to the cluster's nodes and wait for the result.
            @type test: Test 
            @param test: a Test object containing the test's parameters (nodes, matrix, 
                        tasks etc)
        """
       
        self.safe_print("**************** Start Test %s *****************"%test.name)

        checker = Checker() 
        checker.add_banned_thread(current_thread())
        num_nodes = test.num_nodes
        matrix_size = len(test.matrix1)
        block_size = test.get_stored_block_dim()
        
        nodes = []
        data_stores = [] 
        n = matrix_size / block_size

        for i in range(n):
            for j in range(n):

                # create data store and its blocks
                
                block1 = []
                block2 = []

                for k in range(block_size):
                    block1.append(test.matrix1[i*block_size+k][j*block_size:(j+1)*block_size])
                    block2.append(test.matrix2[i*block_size+k][j*block_size:(j+1)*block_size])

                data_store = DataStore(block1, block2, 5, 0, 0, checker)
                data_stores.append(data_store)
            
                # create nodes
                nodes.append(Node(  (i,j),      # node_ID
                                    block_size, 
                                    matrix_size, 
                                    data_store))

        # the nodes need to know about each other

        for node in nodes:
            node.set_nodes(nodes)

        # send tasks to nodes

        tasks = test.tasks
        threads = []
        for task in tasks:
            if len(task) != 5: 
                break
            index = task[0][0] * n + task[0][1]
            t = Thread(target=Tester.start_node, args=(self, nodes[index], task, test))
            checker.add_banned_thread(t)
            
            t.start()
            threads.append(t)
            
            
        for t in threads:
            t.join()

        for node in nodes:
            node.shutdown()
        
        errors = checker.status()
        for error in errors:
            self.print_error(error)

        if not len(errors) and self.passed.is_set():
            self.passed_tests += 1
            self.safe_print("Correct result")
        
        self.safe_print("**************** End Test %s *******************\n" % test.name)

                        
    def start_node(self, node, task, test):
        """
            Sends a task to a cluster's node and checks the result.
            @type node: Node
            @param node: the node to which to send the task
            @type task: Task
            @param task: the task to be performed by the node
            @type test: Test
            @param test: a reference to the whole test, used for checking the result
        """
        
        result = node.compute_matrix_block(task[1], task[2], task[3], task[4])
        
        err = self.check_result(result, test.matrix1, test.matrix2, task)            

        if err:
            self.print_error(err)
        else:
            self.passed.set()
            
    def check_result(self, result_block, matrix1, matrix2, task):
        """
            Checks if a given matrix block is the correct result.
            @type result_block: a list of lists
            @param result_block: the block from the result matrix that was computed by the node
            @type matrix1: a list of lists
            @param matrix1: the first matrix of the current job (test)
            @type matrix2: a list of lists
            @param matrix2: the second matrix of the current job (test)
            @type task: Task
            @param task: the node's task            
        """

        error_message = ""

        if not result_block:
            error_message = "No result matrix block"

        # compute correct result for matrix multipliction

        n = len(matrix1)
        start_row = task[1]
        start_col = task[2]
        num_rows_block = task[3]
        num_cols_block = task[4] 

        correct_result = [[0 for i in range(num_cols_block)] for j in range(num_rows_block)]

        for i in range(start_row, start_row+num_rows_block):
            for j in range(start_col, start_col+num_cols_block):
                for k in range(n):
                    correct_result[i-start_row][j-start_col] += matrix1[i][k] * matrix2[k][j]
    
        # check if nodes' result is correct
        if DEBUG:
            with self.print_lock:    
                self.mprint(correct_result)
                self.mprint(result_block)
    
        if len(correct_result) != len(result_block):
            error_message += "Invalid number of rows for result matrix"
        elif len(result_block)==0:
            error_message += "Empty result matrix"
        else: 
            for i in range(num_rows_block):
                if len(result_block[i]) != num_cols_block:
                    error_message += "Invalid number of columns for result matrix"
                    break
                for j in range(num_cols_block):
                    if (correct_result[i][j] != result_block[i][j]):
                        error_message += "Incorrect result"
                        break
                else:
                    continue
                break

        return error_message
        
    def mprint(self,matrix):
        n = len(matrix)
        for i in range(n):
            for j in range(len(matrix[i])):
                print matrix[i][j]," ",
            print 

        print "--------------------------"
 
    def print_error(self, error_msg):
        with self.print_lock:
            print "ERROR: %s" % error_msg  

    def safe_print(self, message):
        with self.print_lock:
            print message
    
class Test:
    """ Represents a test"""
    
    def __init__(self, name, matrix1, matrix2, num_nodes, tasks, timeout):
        self.name = name
        self.matrix1 = matrix1
        self.matrix2 = matrix2
        self.num_nodes = num_nodes
        self.tasks = tasks
        self.timeout = timeout        

    def get_stored_block_dim(self):
        return len(self.matrix1)/int(math.sqrt(self.num_nodes))



class TestGenerator:
    """
        Randomly generates tests, tests are composed of a matrix, a number of
        nodes and a number of tasks.

        For loading a test's parameters from a file, the following format must be respected:
        
        param_name1 = value

        param_name2 = value

        [...]

        Parameter names are defined in this class. Parameters can be declared in any order in the file.
    """
    
    NUM_NODES = "num_nodes" 
    NUM_TASKS = "num_tasks" 
    STORED_BLOCK_SIZE = "stored_block_size" 
    TEST_NAME = "test_name" 
    TIMEOUT_PERIOD = "timeout" 
    
    def __init__(self, seed = 0):
        self.rand_gen = random.Random(seed)
       
        self.test_template = {}
        self.test_template[self.NUM_NODES] = None
        self.test_template[self.NUM_TASKS] = None
        self.test_template[self.STORED_BLOCK_SIZE] = None
        self.test_template[self.TEST_NAME] = None
        self.test_template[self.TIMEOUT_PERIOD] = None 

    def generate_matrix(self, dim):
        """
            Randomly generates the elemens of a square matrix.
            @type dim: integer
            @param dim: the dimension of the matrix (number of rows = number of columns = dim)
            @return: a list of lists containing the matrix
        """
        matrix = []
        for i in range(dim):
            matrix.append([self.rand_gen.randint(10,100) for i in range(dim)])
        
        return matrix
         

    def generate_test_from_file(self, filename):
        """
            Loads test parameters from a file.
            @return: a Test object
        """
        try:
            f = open(filename, "r")
            #TODO           
            params = dict(self.test_template)
            for line in f:
                if len(line.strip()) == 0:
                    continue
                parts = [i.strip() for i in re.split("=", line)]
                print parts
                if len(parts) == 0:
                    continue
                if len(parts) != 2:
                    print "Wrong test file format" 
                    os._exit(0)
                params[parts[0]] = parts[1]
            f.close()
        except IOError, err:
            print err
            f.close()
            os._exit(0)

        missing = 0
        for p in params.keys():
            if params[p] == None:      
                print  "Wrong test file format - missing parameter %s"%p
                missing+=1            
        if missing: os._exit(0)
            
        num_nodes = int(params[self.NUM_NODES])
        num_tasks = int(params[self.NUM_TASKS])
        stored_block_size = int(params[self.STORED_BLOCK_SIZE])
        test_name = params[self.TEST_NAME]
        timeout = int(params[self.TIMEOUT_PERIOD])

        return self.generate_test(test_name, 
                                num_nodes, 
                                num_tasks, 
                                stored_block_size, 
                                timeout)

    def generate_random_test(self):
        """
            Randomly generates test parameters.
            @return: a Test object
        """

        n = self.rand_gen.randint(2, 15) 
        num_nodes = n * n 
        num_tasks = self.rand_gen.randint(num_nodes, num_nodes + 30)
        stored_block_size = self.rand_gen.randint(3, 10)
        
        return self.generate_test("Random", 
                                num_nodes, 
                                num_tasks, 
                                stored_block_size, 
                                self.rand_gen.randint(60, 100))
    
    def generate_test(self, test_name, num_nodes, num_tasks, stored_block_size, timeout):
        """
            Generates all the test's components (matrix, tasks) based on the given test parameters.
            @type num_nodes: Integer
            @param num_nodes: number of nodes involved in the test
            @type num_tasks: Integer
            @param num_tasks: number of tasks of the test's job
            @type stored_block_size: Integer
            @param stored_block_size: the size of blocks stored in each node's datastore
            @type timeout: Integer
            @param timeout: the number of seconds after which the test is cancelled if it wasn't finished
        """
        mat_dim = int(math.sqrt(num_nodes)) * stored_block_size
        tasks = []
        n = mat_dim / stored_block_size 
        for k in range(num_tasks):
            num_rows_block = self.rand_gen.randrange(3, 5, 1)
            num_cols_block = self.rand_gen.randrange(3, 5, 1)
            start_row = self.rand_gen.randrange(0, mat_dim-num_rows_block)
            start_col = self.rand_gen.randrange(0, mat_dim-num_cols_block)
            
            # assign random nodes to task
            if k < num_nodes: #each node has at least one task
                i = k/n
                j = k%n
                if DEBUG: print "(%d, %d)"%(i,j)
            else: 
                i = self.rand_gen.randrange(0, n)
                j = self.rand_gen.randrange(0, n)
            node_ID = (i,j)
            tasks.append((node_ID, start_row, start_col, num_rows_block, num_cols_block))
                    
        matrix1 = self.generate_matrix(mat_dim)
        matrix2 = self.generate_matrix(mat_dim)
        
        return Test(test_name, matrix1, matrix2, num_nodes, tasks, timeout)
        
        
def usage(argv):
    print "Usage: python %s [OPTIONS]"%argv[0]
    print "options:"
    print "\t-f,   --testfile\ttest file, if not specified it generates a random test"
    print "\t-o,   --out\t\toutput file"
    print "\t-t,   --times\t\tthe number of times the test is run, defaults to 2"
    print "\t-h,   --help\t\tprint this help screen"

if __name__ == "__main__":
    try:
        opts, args = getopt.getopt(sys.argv[1:], "h:f:o:t:", ["help",
            "testfile=", "out=", "times="])
    except getopt.GetoptError, err:
        print str(err)
        usage(sys.argv)
        sys.exit(2)

    test_file = ""
    times = 2
    output_file = "tester.out"

    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            sys.exit(0)
        elif o in ("-f", "--testfile"):
            test_file = a
        elif o in ("-o", "--out"):
            output_file = a 
        elif o in ("-t", "--times"):
            try:
                times = int(a)
            except TypeError, err:
                print str(err)
                sys.exit(2)
        else:
            assert False, "unhandled option"
    
    t = Tester(output_file)
    t.run_test(test_file, times)
