#!/bin/sh
mpicc -o mcdata/mc.exe monte-carich.c -lm
mpicc -o rgdata/rg.exe runger.c -lm
