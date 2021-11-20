// Copyright 2021 Harutyunyan Gurgen
#include <gtest/gtest.h>
#include <cmath>
#include <functional>
#include "./rectangle_method_integration.h"
#include <gtest-mpi-listener.hpp>

TEST(Paralel_Operations_MPI, x_squad)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int a = 1,  b = 10, n = 100;
    const std::function<double(double)>f = [](double x) {return x*x; };
    double sum = getParallelOperations(a, b, n, f);
    
    if (rank == 0)
    {
        double seq_sum = getSequentialOperations(a, b, n, f);
        ASSERT_DOUBLE_EQ(seq_sum, sum);
    }
}
TEST(Paralel_Operations_MPI, x_lnx)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int a = 2, b = 5, n = 100;
    const std::function<double(double)>f = [](double x) {return 1/log(x); };
    double sum = getParallelOperations(a, b, n, f);
    
    if (rank == 0)
    {
        double seq_sum = getSequentialOperations(a, b, n, f);
        ASSERT_DOUBLE_EQ(seq_sum, sum);
    }
}
TEST(Paralel_Operations_MPI, x_sin)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int a = 0, b = 1, n = 100;
    const std::function<double(double)>f = [](double x) {return (1-sin(x)); };
    double sum = getParallelOperations(a, b, n, f);
    
    if (rank == 0)
    {
        double seq_sum = getSequentialOperations(a, b, n, f);
        ASSERT_DOUBLE_EQ(seq_sum, sum);
    }
}
TEST(Paralel_Operations_MPI, x_cos)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int a = 0, b = 1, n = 100;
    const std::function<double(double)>f = [](double x) {return (cos(x)-1); };
    double sum = getParallelOperations(a, b, n, f);
    
    if (rank == 0)
    {
        double seq_sum = getSequentialOperations(a, b, n, f);
        ASSERT_DOUBLE_EQ(seq_sum, sum);
    }
}
TEST(Paralel_Operations_MPI, x_cube)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int a = -3, b = 9, n = 100;
    const std::function<double(double)>f = [](double x) {return x * x*x; };
    double sum = getParallelOperations(a, b, n, f);
   
    if (rank == 0)
    {
        double seq_sum = getSequentialOperations(a, b, n, f);
        ASSERT_DOUBLE_EQ(seq_sum, sum);
    }
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
