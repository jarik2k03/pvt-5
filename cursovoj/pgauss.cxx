#include <math.h>
#include "../mpi.h"
#include <iostream>
#include <vector>
using namespace std;

void printMatrixValues(float** arr, int n, int m) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      float num = (fabs(arr[i][j]) < 1e-4) ? 0.0f : arr[i][j];
      cout << num << "\t";
    }
    cout << endl;
  }
  cout << '\n';
}
void printInverseMatrix(float** arr, int n, int m) {
  for (int i = 0; i < n; ++i) {
    for (int j = n; j < m; ++j) {
      printf("%.3f\t", arr[i][j]);
    }
    cout << endl;
  }
  cout << '\n';
}

void get_chunk(int a, int b, int commsize, int rank, int *lb, int *ub) {
    int n = b - a + 1;
    int q = n / commsize;
    if (n % commsize) q++;
    int r = commsize * q - n;
    
    /* Compute chunk size for the process */
    int chunk = q;
    if (rank >= commsize - r) chunk = q - 1;
    *lb = a; /* Determine start item for the process */
    
    if (rank > 0) { /* Count sum of previous chunks */
        if (rank <= commsize - r)
            *lb += q * rank;
        else
            *lb += q * (commsize - r) + (q - 1) * (rank - (commsize - r));
    }
    *ub = *lb + chunk - 1;
}

void findInvMatGaussJordan(float** mat, int order) {
  int commrank, commsize;
  MPI_Comm_rank(MPI_COMM_WORLD, &commrank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  
  float temp = 0.0f;

  // printMatrixValues(mat, order, order);
  // printInverseMatrix(mat, order, 2 * order);
  //  заполнение правой матрицы
  for (int i = 0; i < order; ++i) {
    mat[i][order + i] = 1;
  }
  cout << "Filled E matrix = \n";
  // printMatrixValues(mat, order, order);
  // printInverseMatrix(mat, order, 2 * order);
  //  сортировка по 0 элементу
  double time = MPI_Wtime();
  for (int i = order - 1; i > 0; --i) {
    if (mat[i - 1][0] < mat[i][0]) {
      float* temp = mat[i];
      mat[i] = mat[i - 1];
      mat[i - 1] = temp;
    }
  }
  cout << "Sorted matrix: " << MPI_Wtime() - time << " sec.\n";
  // printMatrixValues(mat, order, order);
  // printInverseMatrix(mat, order, 2 * order);
  //  вычитание строк
  int lb, ub;
  get_chunk(0, order - 1, commsize, commrank, &lb, &ub);
  time = MPI_Wtime();
  for (int i = 0; i < order; ++i) {
    for (int j = 0; j < order; ++j) {
      if (j == i)
        continue; 
      temp = mat[j][i] / mat[i][i];
      for (int k = 0; k < 2 * order; ++k) {
        mat[j][k] -= mat[i][k] * temp;
      }
      
    }
  }
  cout << "Processed matrix: " << MPI_Wtime() - time << " sec.\n";
  // printMatrixValues(mat, order, order);
  // printInverseMatrix(mat, order, 2 * order);
  //  деление для получения единичной
  time = MPI_Wtime();
  for (int i = 0; i < order; ++i) {
    temp = mat[i][i];
    for (int j = 0; j < 2 * order; ++j) {
      mat[i][j] = mat[i][j] / temp;
    }
  }
  cout << "Divided matrix: " << MPI_Wtime() - time << " sec.\n";
  if (commrank == 0) {
    printMatrixValues(mat, order, order);
    printInverseMatrix(mat, order, 2 * order);
  }

}
int main(int argc, char** argv) {
  
  int commrank, commsize;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &commrank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);

  int order = (argc > 1 && argv[1] != nullptr) ? atoi(argv[1]) : 3;

  int lb, ub; // Декомпозиция матрицы на горизонтальные полосы
  get_chunk(0, order - 1, commsize, commrank, &lb, &ub);
  int nrows = ub - lb + 1;
  cout << "NROWS : " << nrows << " UB: " << ub << " LB: " << lb << '\n';

  int matrix_size = (nrows * order) << 1;
  float** mat = new float*[nrows];
  for (int i = 0; i < order; ++i)
    mat[i] = new float[order * 2];

  for (int i = 0; i < nrows; ++i) {
    for (int j = 0; j < order; ++j) {
      mat[i][j] = rand() % 16;
    }
  }
  findInvMatGaussJordan(mat, order);

  
  for (int i = 0; i < order; ++i)
    delete[] mat[i];
  MPI_Finalize();
  
  return 0;
}