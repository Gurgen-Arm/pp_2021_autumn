// Copyright 2021 Lazarev Alexey

#include "../../../modules/task_3/lazarev_a_ccs_ccs/CCS.h"

std::vector<double> random_matrix(int rows, const int cols) {
  if (rows < 1 || cols < 1) {
    MPI_ERRORS_RETURN;
  }
  int size = rows * cols;
  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_real_distribution<> urd(-100, 100);
  std::vector<double> matrix(size);
  for (int i = 0; i < size; i++) {
    int urd_gen = urd(gen);
    if (urd_gen > 10 || urd_gen < -10) {
      matrix[i] = 0;
    } else {
      matrix[i] = urd_gen;
    }
  }
  return matrix;
}

matrix_CCS build_data_of_matrix_CCS(const std::vector<double>& _matrix,
                                    const int& _rows, const int& _cols) {
  matrix_CCS matrix;
  matrix.cols = _cols;
  matrix.rows = _rows;
  matrix.non_zero = 0;
  int counter;
  matrix.ColumnIndexes.push_back(0);
  for (int index = 0; index < matrix.cols; index++) {
    counter = 0;
    for (int i = index; i < matrix.rows * matrix.cols; i += matrix.cols) {
      if (_matrix[i] != 0) {
        matrix.value.push_back(_matrix[i]);
        matrix.Rows.push_back(i / matrix.cols);
        counter++;
      }
    }
    matrix.ColumnIndexes.push_back(matrix.ColumnIndexes.back() + counter);
    matrix.non_zero += counter;
  }
  return matrix;
}

std::vector<double> multiplication(const matrix_CCS& A, const matrix_CCS& B) {
  if (A.cols != B.rows) {
    MPI_ERRORS_RETURN;
  }
  int rows = A.rows;
  int cols = B.cols;
  int size = rows * cols;
  std::vector<double> matrix(size, 0);

  for (int index = 0; index < A.cols; index++) {
    for (int ind = 0; ind < B.cols; ind++) {
      for (int i = A.ColumnIndexes[index]; i < A.ColumnIndexes[index + 1];
           i++) {
        for (int j = B.ColumnIndexes[ind]; j < B.ColumnIndexes[ind + 1]; j++) {
          if (index == B.Rows[j]) {
            matrix[A.Rows[i] * B.cols + ind] += A.value[i] * B.value[j];
          }
        }
      }
    }
  }
  return matrix;
}

std::vector<double> parallel_multiplication(matrix_CCS& A, matrix_CCS& B) {
  int ProcRank, ProcNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  MPI_Bcast(&A.cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&A.rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&A.non_zero, 1, MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Bcast(&B.cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&B.rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&B.non_zero, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (ProcRank > 0) {
    A.value.resize(A.non_zero);
    A.Rows.resize(A.non_zero);
    A.ColumnIndexes.resize(A.cols + 1);
  }

  MPI_Bcast(A.value.data(), A.non_zero, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(A.Rows.data(), A.non_zero, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(A.ColumnIndexes.data(), A.cols + 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (ProcRank > 0) {
    B.value.resize(B.non_zero);
    B.Rows.resize(B.non_zero);
    B.ColumnIndexes.resize(B.cols + 1);
  }

  MPI_Bcast(B.value.data(), B.non_zero, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(B.Rows.data(), B.non_zero, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(B.ColumnIndexes.data(), B.cols + 1, MPI_INT, 0, MPI_COMM_WORLD);

  std::vector<int> bounds(ProcNum + 1);
  bounds[0] = 0;
  for (int i = 1; i < ProcNum; i++) {
    if (i < A.cols % ProcNum) {
      bounds[i] += A.cols / ProcNum + 1 + bounds[i - 1];
    } else {
      bounds[i] += A.cols / ProcNum + bounds[i - 1];
    }
  }
  bounds[ProcNum] = A.cols;

  std::vector<double> local_matrix(A.rows * B.cols);

  for (int index = bounds[ProcRank]; index < bounds[ProcRank + 1]; index++) {
    for (int ind = 0; ind < B.cols; ind++) {
      for (int i = A.ColumnIndexes[index]; i < A.ColumnIndexes[index + 1];
           i++) {
        for (int j = B.ColumnIndexes[ind]; j < B.ColumnIndexes[ind + 1]; j++) {
          if (B.Rows[j] == index) {
            local_matrix[A.Rows[i] * B.cols + ind] += A.value[i] * B.value[j];
          }
        }
      }
    }
  }

  std::vector<double> global_matrix(A.rows * B.cols);
  MPI_Reduce(local_matrix.data(), global_matrix.data(), A.rows * B.cols,
             MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  return global_matrix;
}
