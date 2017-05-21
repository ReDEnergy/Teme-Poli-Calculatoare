#!/bin/bash

[[ -z $COMPILER ]] && COMPILER="gcc"
if [[ $COMPILER="gcc" ]]; then
   make run_quad
fi
