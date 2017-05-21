#!/usr/bin/python
# -*- coding: utf-8 -*-
# Tudor Berariu, 2014

from pylab import *
from radius import *

x = arange(1, 1001, 1)
y = [radius(i, 1000, 20, 20) for i in x]
plot(x,y)
show()
