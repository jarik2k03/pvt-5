#!/bin/sh
sbatch --output=1x2 t1x2.job
sbatch --output=1x4 t1x4.job
sbatch --output=1x6 t1x6.job
sbatch --output=1x8 t1x8.job
sbatch --output=2x2 t2x2.job
sbatch --output=2x4 t2x4.job
sbatch --output=2x6 t2x6.job
sbatch --output=2x8 t2x8.job