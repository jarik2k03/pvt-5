#include <math.h>
#include <sys/time.h>
#include <iostream>
#include <vector>
using namespace std;

double wtime() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

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
void findInvMatGaussJordan(float** mat, int order) {
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
  double time = wtime();
  for (int i = order - 1; i > 0; --i) {
    if (mat[i - 1][0] < mat[i][0]) {
      float* temp = mat[i];
      mat[i] = mat[i - 1];
      mat[i - 1] = temp;
    }
  }
  cout << "Sorted matrix: " << wtime() - time << " sec.\n";
  // printMatrixValues(mat, order, order);
  // printInverseMatrix(mat, order, 2 * order);
  //  вычитание строк
  time = wtime();
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
  cout << "Processed matrix: " << wtime() - time << " sec.\n";
  // printMatrixValues(mat, order, order);
  // printInverseMatrix(mat, order, 2 * order);
  //  деление для получения единичной
  time = wtime();
  for (int i = 0; i < order; ++i) {
    temp = mat[i][i];
    for (int j = 0; j < 2 * order; ++j) {
      mat[i][j] = mat[i][j] / temp;
    }
  }
  cout << "Divided matrix: " << wtime() - time << " sec.\n";
  // printMatrixValues(mat, order, order);
  // printInverseMatrix(mat, order, 2 * order);
}
int main(int argc, const char** argv) {
  int order = (argc > 1 && argv[1] != nullptr) ? atoi(argv[1]) : 3;
  int matrix_size = (order * order) << 1;
  float** mat = new float*[order];
  for (int i = 0; i < order; ++i)
    mat[i] = new float[order * 2];

  for (int i = 0; i < order; ++i) {
    for (int j = 0; j < order; ++j) {
      mat[i][j] = rand() % 16;
    }
  }
  findInvMatGaussJordan(mat, order);

  for (int i = 0; i < order; ++i)
    delete[] mat[i];

  return 0;
}