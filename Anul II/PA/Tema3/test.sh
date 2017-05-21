#!/bin/bash
for i in {1..16}
do
	echo -e "\n\n>>>>> Test $i"
	time ./tema3 ./teste/$i
	diff -w ./teste/$i/example.pgm ./teste/$i/segment.pgm
	echo "<<<<<"
done
