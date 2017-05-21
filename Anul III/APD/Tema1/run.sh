#!/bin/bash

TEST_DIR="."

NAME=$1
TIME_FRAME=$2
FIN=$3
FOUT=$4
THREADS=$5
TYPE=$6
CHUNK_SIZE=$7

sch[0]="static"
sch[1]="dynamic"
sch[2]="guided"

SCHEDULE="static"

if [ $# -eq 6 ]; then
	SCHEDULE=${sch[$TYPE]}
fi

if [ $# -eq 7 ]; then
	SCHEDULE=${sch[$TYPE]},$CHUNK_SIZE
fi

if [ $# -ge 5 ]; then
	export OMP_NUM_THREADS=$THREADS
	export OMP_SCHEDULE=$SCHEDULE
	echo "THREADS: "$THREADS" SCHEDULE: "$SCHEDULE
fi

time ./$NAME $TIME_FRAME $TEST_DIR/$FIN $FOUT
