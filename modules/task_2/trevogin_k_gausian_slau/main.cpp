// Copyright 2021 Trevogin Kirill
#include "./gausian_slau.h"

TEST(Parallel_Operations_MPI, Test_Gaus_metod_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> mat;
    std::vector<double> vec;
    std::vector<double> vec1;
    std::vector<double> vec2;
    vec = getRandomVector(&mat, 4);
    gaus_metod(mat, vec);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        vec1 = mat;
        vec2 = mat;
        ASSERT_EQ(vec1, vec2);
    }
}
TEST(Parallel_Operations_MPI, Test_Gaus_metod_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> mat;
    std::vector<double> vec;
    std::vector<double> vec1;
    std::vector<double> vec2;
    vec = getRandomVector(&mat, 9);
    gaus_metod(mat, vec);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        vec1 = mat;
        vec2 = mat;
        ASSERT_EQ(vec1, vec2);
    }
}
TEST(Parallel_Operations_MPI, Test_Gaus_metod_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> mat;
    std::vector<double> vec;
    std::vector<double> vec1;
    std::vector<double> vec2;
    vec = getRandomVector(&mat, 0);
    gaus_metod(mat, vec);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        vec1 = mat;
        vec2 = mat;
        ASSERT_EQ(vec1, vec2);
    }
}
TEST(Parallel_Operations_MPI, Test_Gaus_metod_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> mat;
    std::vector<double> vec;
    std::vector<double> vec1;
    std::vector<double> vec2;
    vec = getRandomVector(&mat, 1);
    gaus_metod(mat, vec);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        vec1 = mat;
        vec2 = mat;
        ASSERT_EQ(vec1, vec2);
    }
}
TEST(Parallel_Operations_MPI, Test_Gaus_metod_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> mat;
    std::vector<double> vec;
    std::vector<double> vec1;
    std::vector<double> vec2;
    vec = getRandomVector(&mat, 20);
    gaus_metod(mat, vec);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        vec1 = mat;
        vec2 = mat;
        ASSERT_EQ(vec1, vec2);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
