#!/bin/bash

EXE=~/APD/Tema4/Tema4

EXE=$1
TOP=$2
MSG=$3
PROCESSES=$4
DBG=$5

if [ $# -le 3 ]; then
    PROCESSES=12
fi

if [ $# -le 4 ]; then
    DBG=0
fi

module load libraries/openmpi-1.6-gcc-4.4.6

time mpirun -n $PROCESSES $EXE $TOP $MSG $DBG
 
 