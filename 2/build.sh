#!/bin/sh
mpicc  -o mc.exe monte-carich.c -lm -lmpich
mpicc  -o rg.exe runger.c -lm -lmpich