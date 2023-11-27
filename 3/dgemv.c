#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

enum { m = 45000, n = 45000 };

void dgemv(float *a, float *b, float *c, int m, int n)
{
    for (int i = 0; i < m; i++) {
        c[i] = 0.0;
        for (int j = 0; j < n; j++)
            c[i] += a[i * n + j] * b[j];
    }
}

int main(int argc, char **argv)
{
    FILE* out = fopen("serial", "w");
    printf("Memory used: %" PRIu64 " MiB\n", (u_int64_t)(((float)m * n + m + n) * sizeof(float)) >> 20);
    float t = MPI_Wtime();
    float *a, *b, *c;
    a = malloc(sizeof(*a) * m * n);
    b = malloc(sizeof(*b) * n);
    c = malloc(sizeof(*c) * m);

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            a[i * n + j] = i + 1;
    }

        for (int j = 0; j < n; j++)
            b[j] = j + 1;
    
    dgemv(a, b, c, m, n);
    t = MPI_Wtime() - t;
    
    // Validation
    for (int i = 0; i < m; i++) {
        float r = (i + 1) * (n / 2.0 + pow(n, 2) / 2.0);
        if (fabs(c[i] - r) > 1E-6) {
            //fprintf(stderr, "Validation failed: elem %d = %f (real value %f)\n", i, c[i], r); break;
        }
    }

    float gflop = 2.0 * m * n * 1E-9;
    fprintf(out, "%.6f\n", t);
    
    free(a); free(b); free(c);
    
    return 0;
}