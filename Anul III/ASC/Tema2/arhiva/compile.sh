#!/bin/bash
#
# Author: Gabriel Ivanica
#

make clean

module load libraries/atlas-3.10.1-gcc-4.4.6-opteron
make opteron

module load libraries/atlas-3.10.1-gcc-4.4.6-nehalem
make nehalem

module load libraries/atlas-3.10.1-gcc-4.4.6-quad
make quad