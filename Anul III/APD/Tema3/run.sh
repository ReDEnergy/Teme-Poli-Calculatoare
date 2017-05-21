#!/bin/bash

DOUT=~/APD/Tema3/
TEST=~/APD/Tema3/test

EXE=$1
FIN=$2
FOUT=$3
PROCESSES=$4

if [ $# -le 3 ]; then
    PROCESSES=1
fi

module load libraries/openmpi-1.6-gcc-4.4.6

time mpirun -n $PROCESSES $EXE $TEST/$FIN $DOUT$FOUT

echo "IMG : $FOUT - Processes : $PROCESSES"
