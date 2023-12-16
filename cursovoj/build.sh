#!/bin/sh
/usr/bin/mpic++ -g3 -O0 -o data/serial.exe gauss.cxx -lm
/usr/bin/mpic++ -g3 -O0 -o data/parallel.exe pgauss.cxx -lm
gcc -o data/util.exe calc_speedup.c 
