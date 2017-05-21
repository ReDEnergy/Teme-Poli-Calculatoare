This directory contains the Fundamental Clustering Problems Suite (FCPS).
It conatains different data sets with a given classification.

The data sets are rather simple and low in dimensionality.
Each data set adresses a certain problem for clustering.

Standard clusteringalgorithms such as SingleLinkage, Wards and k-means 
are not able to cluster all of the data sets correctly.

This FCPS may serve as a minimal test for new invented cluster algorithms.

Every new invented algorithm should at least cluster the FCPS correctly.

The data sets may be used as long as the following citation is referenced in any publication
concering any of this data:

Alfred Ultsch, Clustering with SOM: U*C, in Proc. Workshop on Self Organizing Feature Maps ,pp 31-37 Paris 2005.



The data sets and main problems adressed are:

data set	main problems of the set
Hepta      	none, i.e clear defined clusters               
Lsun       	different variances and inter cluster distances
Tetra      	almost touching clusters                       
Chainlink  	linear not separable                           
Atom       	different variances and linear not separable   
EngyTime   	gaussian mixture                               
Target     	outlier                                        
TwoDiamonds	cluster border defined by density              
WingNut    	density vs. distance                           
GolfBall   	no cluster at all       


description of the file formats:
All files are ASCII text files. Colums are separated by TAB. Headers are included.

*.lrn  the data including a unique key for each case 
*.cls  contains key and class. Each class is indicated ba a positive number.
