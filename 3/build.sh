#!/bin/sh
mpicc -o data/dgemv.exe dgemv.c -lm
mpicc -o data/sgemv.exe sgemv.c -lm
gcc -o data/util.exe calc_speedup.c 