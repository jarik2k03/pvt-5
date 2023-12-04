#!/bin/sh
mpicc -o data/serial.exe grid2.c -lm
mpicc -o data/parallel.exe pgrid2.c -lm
gcc -o data/util.exe calc_speedup.c 
