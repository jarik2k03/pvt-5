#!/bin/sh
mpicc -o data/dgemv.exe dgemv.c -lm
mpicc -o data/sgemv.exe sgemv.c -lm
