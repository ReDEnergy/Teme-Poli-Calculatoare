The Fundamental Clustering Problems Suite (FCPS) contains different data 
sets with known classification. The data sets are rather simple and low 
in dimensionality. Each data set adresses a certain problem for clustering. 
Standard clustering algorithms such as SingleLinkage, Ward and k-means are 
not able to cluster all of the data sets correctly. This FCPS may serve as 
a minimal test for newly invented cluster algorithms. Every new algorithm 
should at least cluster the FCPS correctly.

If you use the data sets in your scientific work, please cite the following 
reference:

Ultsch, A.: Clustering with SOM: U*C, 
In Proc. Workshop on Self-Organizing Maps, Paris, France, (2005), pp. 75-82

We would like you to send us links to your publications to
databionics@informatik.uni-marburg.de to create a list of references where 
the data was used.

The data sets and main problems adressed are:

Dataset 	Main problems
Hepta		none, i.e. clearly defined clusters
Lsun		different variances and inter cluster distances
Tetra		almost touching clusters
Chainlink 	linear not separable
Atom		different variances and linear not separable
EngyTime 	Gaussian mixture
Target		outliers
TwoDiamonds 	cluster border defined by density
WingNut 	density vs. distance
GolfBall 	no clusters at all

All files are ASCII text files. Colums are separated by TAB. Headers are 
included. *.lrn files contain the data including a unique key for each case, 
*.cls contain keys and class labels. Each class is indicated by a positive 
number.
