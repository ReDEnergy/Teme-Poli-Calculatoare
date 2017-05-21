# Ivanica Gabriel
# Tema 3 C
# 342 C5

import random
import math
import json
import re
from os import listdir
from collections import defaultdict
from multiprocessing import Pool

import gmpy2
from gmpy2 import mpfr
gmpy2.get_context().precision = 53

spam_files = [
    ["data/easy_ham/", "data/easy_ham_2/", "data/hard_ham/"], 
    ["data/spam/", "data/spam_2/"]]

# Spam Filter
training_size = 0.7      # training set

TrainSet = None
TestSet = None

# Regular expression compiled 
sre = re.compile(r'[^a-zA-Z0-9]')

def reset_globals():
    global TrainSet
    global TestSet
    # Number of docs of class X, Number of words in class X, word dict
    TrainSet = [[0, 0, defaultdict(int)] for _ in xrange(nr_of_classes)]
    
    # [training set, test set]
    TestSet = [[] for _ in xrange(nr_of_classes)]
   
################################################################################
# Naive Bayes
    
def process_words(data, cls):
    global TrainSet
    global TestSet

    words = [x for x in sre.split(data) if x]
    
    D = defaultdict(int)
    r = random.random()
    if r < training_size:
        # Naive Bayes
        TrainSet[cls][0] += 1             # Number of docs of class X 
        for word in words:
            D[word] += 1
            TrainSet[cls][1] += 1         # Number of words in class X
            TrainSet[cls][2][word] += 1   # Repeats of words per class
    else:
        for word in words:
            D[word] += 1
            for i in xrange(nr_of_classes):
                TrainSet[i][2][word]
        TestSet[cls].append(D)

def naive_bayes(TrainSet, TestSet):
    print ("####################################################################")
    print

    # Priors
    NRC = 0
    for cls in xrange(nr_of_classes):
        NRC += TrainSet[cls][0]
    P = [float(TrainSet[cls][0]) / NRC for i in xrange(nr_of_classes)]

    # Compute number of unique training words
    D = {}
    nr_unique_words = 0
    for cls in xrange(nr_of_classes):
        for k, v in TrainSet[cls][2].iteritems():
            D[k] = 1
    nr_unique_words = len(D)

    print ("UNIQUE WORDS:", nr_unique_words)
    print

    # Conditional Probabilities
    PC = [{} for _ in xrange(nr_of_classes)]
    for cls in xrange(nr_of_classes):
        DVD = TrainSet[cls][1] + nr_unique_words
        for k, v in TrainSet[cls][2].iteritems():
            PC[cls][k] = mpfr(v + 1) / DVD
    
    # Training set
    print ("Training size")
    print [TrainSet[cls][0] for cls in xrange(nr_of_classes)]

    # Test Set
    success = [0] * nr_of_classes
    fail = [[0] * nr_of_classes for i in xrange(nr_of_classes)]

    for exp_cls, docs in enumerate(TestSet):
        for doc in docs:
            CLS = -1
            PR = [mpfr(P[cls]) for cls in xrange(nr_of_classes)]
            for cls in xrange(nr_of_classes):
                for k, v in doc.iteritems():
                    PR[cls] *= PC[cls][k] ** v
                
            _ , CLS = max(zip(PR, xrange(nr_of_classes)))
                
            if CLS == exp_cls:
                success[CLS] += 1
            else:
                fail[exp_cls][CLS] += 1

        print
        print "CLS: ", exp_cls, "\tSUCCESS:", success[exp_cls], "(", len(docs), ")"
        print "RATE:", float(success[exp_cls]) * 100 / len(docs)
    confusion_matrix(success, fail)
    print


def confusion_matrix(success, fail):
    print ("================================")
    for cls in xrange(nr_of_classes):
        fail[cls][cls] = success[cls]
        print ("CLS {}: \t {}".format(cls, fail[cls]))
        
# Read data set

def print_set_info(TrainSet, TestSet):
    print "NR Classes", nr_of_classes
    print "Train set:"
    for cls in xrange(nr_of_classes):
        print "\tCLS:", cls, "->", len(TrainSet[cls])

    print "Test set:"
    for cls in xrange(nr_of_classes):
        print "\tCLS:", cls, "->", len(TestSet[cls])

def read_rotten_tomatoes(filename):
    with open(filename, 'r') as f:
        header = f.readline().strip().split("\t")
        data = f.read()

        lines = data.split("\n")
        del lines[-1]
        
        for line in lines:
            line = line.split("\t")
            CLS = line[3]
            process_words(line[2], int(CLS))

def process_spam_file(filename, cls):
    with open(filename, 'r') as f:
        data = f.read()

    process_words(data, cls)

def read_spam_directory(directory, cls):
    files = listdir(directory)
    
    for file in files: 
        process_spam_file(directory + file, cls)

################################################################################
# Main

if __name__ == "__main__":

    # Naive Bayes
    nr_of_classes = 5
    reset_globals()
    read_rotten_tomatoes("data/train.tsv")
    naive_bayes(TrainSet, TestSet)

    # Naive Bayes
    nr_of_classes = 2
    reset_globals()
    for cls, v in enumerate(spam_files):
        for file in v:
            read_spam_directory(file, cls)

    naive_bayes(TrainSet, TestSet)