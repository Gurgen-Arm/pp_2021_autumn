// Copyright 2021 Ershov Alexey
#include <gtest/gtest.h>
// #include <time.h>

#include <gtest-mpi-listener.hpp>

#include "./reduce_mpi.h"

TEST(reduse_test, test_BIG_FLOAT_MAX) {
  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  float* data = getRandomVector<float>(10000);

  float* res = nullptr;
  float* myRes = nullptr;

  if (rank == 0) {
    res = new float[10000];
    myRes = new float[10000];
  }

  // double start = MPI_Wtime();

  reduce(data, myRes, 10000, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);

  // double end = MPI_Wtime();

  /*std::cout << "Time of reduce work: " << (end - start) << " sec " << std::endl;*/

  // double start2 = MPI_Wtime();

  MPI_Reduce(data, res, 10000, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);

  // double end2 = MPI_Wtime();

  /*std::cout << "Time of MPI_reduce work : " << (end2 - start2) << " sec "
            << std::endl;*/

  if (rank == 0) {
    for (size_t i = 0; i < 10000; ++i) {
      ASSERT_EQ(myRes[i], res[i]);
    }
  }

  delete[] res;
  delete[] myRes;
  delete[] data;
}

TEST(reduse_test, test_INT_PROD) {
  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int* data = new int[1000];
  for (size_t i = 0; i < 1000; ++i) {
    data[i] = i;
  }

  int* res = nullptr;
  int* myRes = nullptr;

  if (rank == 0) {
    res = new int[1000];
    myRes = new int[1000];
  }

  reduce(data, myRes, 1000, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);
  MPI_Reduce(data, res, 1000, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    for (size_t i = 0; i < 1000; ++i) {
      ASSERT_EQ(myRes[i], res[i]);
    }
  }

  delete[] res;
  delete[] myRes;
  delete[] data;
}

TEST(reduse_test, test_FLOAT_MIN) {
  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  float* data = getRandomVector<float>(1000);

  float* res = nullptr;
  float* myRes = nullptr;

  if (rank == 0) {
    res = new float[1000];
    myRes = new float[1000];
  }

  reduce(data, myRes, 1000, MPI_FLOAT, MPI_MIN, 0, MPI_COMM_WORLD);
  MPI_Reduce(data, res, 1000, MPI_FLOAT, MPI_MIN, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    for (size_t i = 0; i < 1000; ++i) {
      ASSERT_EQ(myRes[i], res[i]);
    }
  }

  delete[] res;
  delete[] myRes;
  delete[] data;
}

TEST(reduse_test, test_DOUBLE_MAX) {
  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  double* data = getRandomVector<double>(1000);

  double* res = nullptr;
  double* myRes = nullptr;

  if (rank == 0) {
    res = new double[1000];
    myRes = new double[1000];
  }

  reduce(data, myRes, 1000, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(data, res, 1000, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    for (size_t i = 0; i < 1000; ++i) {
      ASSERT_EQ(myRes[i], res[i]);
    }
  }

  delete[] res;
  delete[] myRes;
  delete[] data;
}

TEST(reduse_test, test_INT_SUM) {
  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int* data = new int[1000];
  for (size_t i = 0; i < 1000; ++i) {
    data[i] = i;
  }

  int* res = nullptr;
  int* myRes = nullptr;

  if (rank == 0) {
    res = new int[1000];
    myRes = new int[1000];
  }

  reduce(data, myRes, 1000, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Reduce(data, res, 1000, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    for (size_t i = 0; i < 1000; ++i) {
      ASSERT_EQ(myRes[i], res[i]);
    }
  }

  delete[] res;
  delete[] myRes;
  delete[] data;
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
