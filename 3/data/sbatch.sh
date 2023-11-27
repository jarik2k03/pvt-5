#!/bin/sh
./dgemv.exe
sbatch --output=2x1 t2x1.job
sbatch --output=2x2 t2x2.job
sbatch --output=2x3 t2x3.job
sbatch --output=2x4 t2x4.job
sbatch --output=2x5 t2x5.job
sbatch --output=2x6 t2x6.job
sbatch --output=2x7 t2x7.job
sbatch --output=2x8 t2x8.job
