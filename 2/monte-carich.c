#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double getrand(){ 
    return (double)rand() / RAND_MAX; 
}

double func(double x, double y){
    return exp(x - y);
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int in = 0;
    double s = 0;
    const int n = 1e+8;
    int rank, commsize;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    srand(rank);

    double start_time = MPI_Wtime();

    for(int i = rank; i < n; i += commsize) {
        double x = getrand();
        double y = getrand();
        if (y <= 1) {
            in++;
            s += func(x, y);
        }
    }

    int gin = 0;
    MPI_Reduce(&in, &gin, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    double gsum = 0.0;
    MPI_Reduce(&s, &gsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();
    
    double elapsed_time = end_time - start_time;
    double max_elapsed_time;

    MPI_Reduce(&elapsed_time, &max_elapsed_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double v = 1 * (double)gin / n;
        double res = v * gsum / gin;
        printf("%.6f", max_elapsed_time);
        // printf("Elapsed time = %.5f\n", max_elapsed_time);
        // printf("Result: %.12f, n %d\n", res, n);
    }
    MPI_Finalize();
    return 0;
}
