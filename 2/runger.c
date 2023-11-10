#include <stdio.h>
#include <math.h>
#include <mpi.h>

double func(double x)
{
    return pow(x,4) / (0,5 * pow(x,2) + x + 6);
}

int main(int argc, char **argv)
{
    int commsize, rank;
    float a = 0.4;
    float b = 1.5;
    float eps = 1E-6;
    const int n0 = 1000000;
    int n = n0, k;
    double sq[2];
    double delta = 1;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start_time;
    start_time = MPI_Wtime();

    double t, max_t;
    for (k = 0; delta > eps; n *= 2, k ^= 1) {
        int points_per_proc = n / commsize;
        int lb = rank * points_per_proc;
        int ub = (rank == commsize - 1) ? (n - 1) : (lb + points_per_proc - 1);

        double h = (b - a) / n;
        double s = 0.0;

        for (int i = lb; i <= ub; i++){
            s += func(a + h * (i + 0.5));
        }
        t -= MPI_Wtime();
        MPI_Allreduce(&s, &sq[k], 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        t += MPI_Wtime();
        sq[k] *= h;

        if (n > n0){
            delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
        }
    }

    double elapsed_time = MPI_Wtime() - start_time;
    double max_elapsed_time;
    MPI_Reduce(&t, &max_t, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&elapsed_time, &max_elapsed_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        printf("Elapsed time = %.6f, Time for comm = %.2f ,Result Pi: %.3f; Runge rule: EPS %e, n %d\n", elapsed_time, max_t / max_elapsed_time * 100,sq[k] * sq[k], eps, n / 2);
    }

    MPI_Finalize();
    return 0;
}