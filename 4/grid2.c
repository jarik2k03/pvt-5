#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EPS 0.001
#define PI 3.14159265358979323846
#define IND(i, j) ((i)*nx + (j))

int main(int argc, char *argv[]) {
    FILE* out = fopen("serial", "w");
    int rows = 2000;
    int cols = 2000;
    int ny = rows;
    int nx = cols;
    double *local_grid = calloc(ny * nx, sizeof(*local_grid));
    double *local_newgrid = calloc(ny * nx, sizeof(*local_newgrid));
    double dx = 1.0 / (nx - 1.0);
    // Initialize top border: u(x, 0) = sin(pi * x)
    for (int j = 0; j < nx; j++) {
        int ind = IND(0, j);
        local_newgrid[ind] = local_grid[ind] = sin(PI * dx * j);
    }
    // Initialize bottom border: u(x, 1) = sin(pi * x) * exp(-pi)
    for (int j = 0; j < nx; j++) {
        int ind = IND(ny - 1, j);
        local_newgrid[ind] = local_grid[ind] = sin(PI * dx * j) * exp(-PI);
    }
    double ttotal = -MPI_Wtime();
    int niters = 0;
    for (;;) {
        niters++;
        for (int i = 1; i < ny - 1; i++) {  // Update interior points
            for (int j = 1; j < nx - 1; j++) {
                local_newgrid[IND(i, j)] =
                    (local_grid[IND(i - 1, j)] + local_grid[IND(i + 1, j)] +
                     local_grid[IND(i, j - 1)] + local_grid[IND(i, j + 1)]) *
                    0.25;
            }
        }
        // Check termination condition
        double maxdiff = 0;
        for (int i = 1; i < ny - 1; i++) {
            for (int j = 1; j < nx - 1; j++) {
                int ind = IND(i, j);
                maxdiff =
                    fmax(maxdiff, fabs(local_grid[ind] - local_newgrid[ind]));
            }
        }
        // Swap grids (after termination local_grid will contain result)
        double *p = local_grid;
        local_grid = local_newgrid;
        local_newgrid = p;
        if (maxdiff < EPS) break;
    }
    ttotal += MPI_Wtime();

    printf("# Heat 2D (serial): grid: rows %d, cols %d\n", rows, cols);
    printf("# niters %d, total time %.6f\n", niters, ttotal);
    fprintf(out, "%.6f\n", ttotal);

    return 0;
}