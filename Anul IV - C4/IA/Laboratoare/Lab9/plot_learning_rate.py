#!/usr/bin/python
# -*- coding: utf-8 -*-
# Tudor Berariu, 2014

from pylab import *
from learning_rate import *

x = arange(1, 1001, 1)
y = [learning_rate(i, 1000) for i in x]

plot(x,y)

show()
