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

spam_files = [
    ["data/easy_ham/", "data/easy_ham_2/", "data/hard_ham/"], 
    ["data/spam/", "data/spam_2/"]]

# Spam Filter
training_size = 0.99

TrainSetKNN = None
TestSet = None
KSIZE = 9
success = None
successW = None

# Regular expression compiled 
sre = re.compile(r'[^a-zA-Z0-9]')

def reset_globals():
    global TrainSetKNN    
    global TestSet

    TrainSetKNN = [[] for _ in xrange(nr_of_classes)]
    
    # [training set, test set]
    TestSet = [[] for _ in xrange(nr_of_classes)]
   
################################################################################
# Process words
    
def process_words(data, cls):
    global TestSet
    global TrainSetKNN
    
    words = [x for x in sre.split(data) if x]
    
    D = defaultdict(int)
    for word in words:
        D[word] += 1

    r = random.random()
    if r < training_size:
        TrainSetKNN[cls].append(D)
    else:
        TestSet[cls].append(D)

################################################################################
# K-Nearest Neighbors

def confusion_matrix(success):
    print ("================================")
    for cls in xrange(nr_of_classes):
        print ("CLS {}: \t {}".format(cls, success[cls]))

def dist(dict_a, dict_b):
    dist = 0
    for word, value in dict_a.iteritems():
        if dict_b.get(word, 0):
            dist += 1
    return dist

def classify(doc):
    K = []
    for cls in xrange(nr_of_classes):
        for train_doc in TrainSetKNN[cls]:
            K.append((dist(doc, train_doc), cls))
            if len(K) > KSIZE:
                K.remove(min(K))

    return sorted(K)

def KNN_succes_rate(doc_knn, expected_class):
    global success
    global successW
    CLS = [0] * nr_of_classes
    CLSW = [0] * nr_of_classes
    for K in xrange(KSIZE):
        CLS[doc_knn[K][1]] += 1
        _, rez = max(zip(CLS, [i for i in xrange(nr_of_classes)]))
        success[K][expected_class][rez] += 1

        CLSW[doc_knn[K][1]] += doc_knn[K][0]
        _, rez = max(zip(CLSW, [i for i in xrange(nr_of_classes)]))
        successW[K][expected_class][rez] += 1

   
def KNN(train_set, test_set):
    global success
    global successW
    success = [[[0] * nr_of_classes for _ in xrange(nr_of_classes)] for _ in xrange(KSIZE)]
    successW = [[[0] * nr_of_classes for _ in xrange(nr_of_classes)] for _ in xrange(KSIZE)]

    PP = Pool(4)

    for exp_cls, docs in enumerate(test_set):
        nr_docs = len(docs)
        print
        print ("DOCS: {} \t CLS: {}".format(nr_docs, exp_cls))

        docs_knn = PP.map(classify, docs)

        for doc_knn in docs_knn:
            KNN_succes_rate(doc_knn, exp_cls)
        for i in xrange(KSIZE):
            print "K", i, round(float(success[i][exp_cls][exp_cls]) * 100/nr_docs, 2), "%"
            print "KW", i, round(float(successW[i][exp_cls][exp_cls]) * 100/nr_docs, 2), "%"
        
#    for i in xrange(KSIZE):
#        print "K: ", i
#        confusion_matrix(success[i])
#        confusion_matrix(successW[i])
    
    PP.close()
        
################################################################################
# Read data set

def print_set_info(train_set, test_set):
    print "NR Classes", nr_of_classes
    print "Training set:"
    for cls in xrange(nr_of_classes):
        print "\tCLS:", cls, "->", len(train_set[cls])

    print "Test set:"
    for cls in xrange(nr_of_classes):
        print "\tCLS:", cls, "->", len(test_set[cls])

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

    # TEST SET 1
    nr_of_classes = 5
    reset_globals()
    read_rotten_tomatoes("data/train.tsv")
 
    print_set_info(TrainSetKNN, TestSet)
    KNN(TrainSetKNN, TestSet) 
        
    # TEST SET 2
    nr_of_classes = 2
    reset_globals()
    for cls, v in enumerate(spam_files):
        for file in v:
            read_spam_directory(file, cls)
  
    print "\nK-Nearest Neighbors"
    print_set_info(TrainSetKNN, TestSet)
    KNN(TrainSetKNN, TestSet) 
    
