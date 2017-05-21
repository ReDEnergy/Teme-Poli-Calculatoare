#!/usr/bin/python
# -*- coding: utf-8 -*-
# Tudor Berariu, 2014

from PIL import Image
import copy
import sys
import random
from learning_rate import *
from radius import *
from neighbourhood import *

def rand_rgb():
    r = random.random()
    g = random.random()
    b = random.random()
    
    return [r, g, b]

def dist2(x, y):
    return (x[0] - y[0]) **2 + (x[1] - y[1]) **2 + (x[2] - y[2]) **2

def argmin(W, x, n):
    min = [1000000, -1, -1]
    for i in xrange(n):
        for j in xrange(n):
            d2 = dist2(W[i][j], x)
            if d2 < min[0]:
                min = [d2, i, j]
    return min
                
def som_segmentation(orig_file_name, n):
    ## După rezolvarea Exercițiilor 2, 3 și 4
    ## în fișierele learning_rate.py, radius.py și neighbourhood.py
    ## rezolvați aici Exercițiile 5 și 6

    orig_img = Image.open(orig_file_name)
    orig_pixels = list(orig_img.getdata())
    orig_pixels = [(o[0]/255.0, o[1]/255.0, o[2]/255.0) for o in orig_pixels]


    ## Exercițiul 5: antrenarea rețelei Kohonen
    ## Exercițiul 5: completați aici:

    W = [[rand_rgb() for _ in xrange(int(n))] for _ in xrange(int(n))]
    t = 1
    maxt = 100

    for t in xrange(maxt):
        rand_px = random.choice(orig_pixels)
        
        min = argmin(W, rand_px, n)
        mask = neighbourhood(min[1], min[2], radius(t, maxt, n, n), n, n)
        
        learn_rate = learning_rate(t, maxt)
        for i in xrange(n):
            for j in xrange(n):
                if mask[i][j]:
                    for k in xrange(3):
                        W[i][j][k] += learn_rate * (rand_px[k] - W[i][j][k])

    ## Exercițiul 5: ----------

    ## Exercițiul 6: crearea imaginii segmentate pe baza ponderilor W
    ## Exercițiul 6: porniți de la codul din show_neg
    ## Exercițiul 6: completați aici:


    seg_img = []
    for px in orig_pixels:
        new_px = None
        min = argmin(W, px, n)
        new_px = W[min[1]][min[2]]
        seg_img.append(tuple([int( o * 255.0) for o in new_px]))
    
    new_img = Image.new('RGB', orig_img.size)
    new_img.putdata(seg_img)
    new_img.show()
    new_img.save('4.jpg')

    ## Exercițiul 6: ----------

if __name__ == "__main__":
    som_segmentation(sys.argv[1], int(sys.argv[2]))
