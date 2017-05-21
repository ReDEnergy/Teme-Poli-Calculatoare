#!/bin/bash

NR_FRAMES=1
DEBUG=0
ERROR=0

INPUT_DIR_SERIAL=./result_serial/
INPUT_DIR_CELL=./result/
NAME=result

EXE=./imgdiff


if [ $# -ge 1 ] 
	then
		NR_FRAMES=$1
fi

if [ $# -eq 2 ] 
	then
		DEBUG=1
fi

echo COMPARE $NR_FRAMES FRAMES

for i in $(seq 1 $NR_FRAMES)
do
	RES=`$EXE $INPUT_DIR_SERIAL$NAME$i.pnm  $INPUT_DIR_CELL$NAME$i.pnm`
	DIFF=${RES:11}
	
	if [ $DEBUG -eq 1 ]
		then
			echo "FRAME $i: DIFF $DIFF"
			if [ $DIFF -gt 0 ]
				then
				ERROR=1
			fi
		elif [ $DIFF -gt 0 ]
			then
				echo "FRAME $i: DIFF $DIFF"
				ERROR=1
	fi
done

if [ $ERROR -eq 0 ]
	then
		echo "ALL FRAMES ARE EQUAL"
fi
		