// Copyright 2021 Pinezhanin Evgeny
#include <gtest/gtest.h>
#include "../../../modules/task_2/pinezhanin_e_bubble_sort_odd_even/bubble_sort_odd_even.h"
#include <gtest-mpi-listener.hpp>

TEST(Bubble_sort_odd_even_test, sort_1000_element) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int size_vector = 1000;
    std::vector<int> vec, ref_vec;

    if (rank == 0) {
        vec = getRandomVector(size_vector);
        ref_vec = vec;
    }

    vec = BubbleSortOddEvenParallel(vec);

    if (rank == 0) {
        ref_vec = BubbleSortOddEvenSequential(ref_vec);
        ASSERT_EQ(ref_vec, vec);
    }
}

TEST(Bubble_sort_odd_even_test, sort_10000_element) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int size_vector = 10000;
    std::vector<int> vec, ref_vec;

    if (rank == 0) {
        vec = getRandomVector(size_vector);
        ref_vec = vec;
    }

    vec = BubbleSortOddEvenParallel(vec);

    if (rank == 0) {
        ref_vec = BubbleSortOddEvenSequential(ref_vec);
        ASSERT_EQ(ref_vec, vec);
    }
}

TEST(Bubble_sort_odd_even_test, sort_15000_element) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int size_vector = 15000;
    std::vector<int> vec, ref_vec;

    if (rank == 0) {
        vec = getRandomVector(size_vector);
        ref_vec = vec;
    }

    vec = BubbleSortOddEvenParallel(vec);

    if (rank == 0) {
        ref_vec = BubbleSortOddEvenSequential(ref_vec);
        ASSERT_EQ(ref_vec, vec);
    }
}

TEST(Bubble_sort_odd_even_test, sorted_vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int size_vector = 1000;
    std::vector<int> vec, ref_vec;

    if (rank == 0) {
        vec.resize(size_vector);
        for (int i = 0; i < size_vector; i++) {
            vec[i] = i;
        }
        ref_vec = vec;
    }

    vec = BubbleSortOddEvenParallel(vec);

    if (rank == 0) {
        ref_vec = BubbleSortOddEvenSequential(ref_vec);
        ASSERT_EQ(ref_vec, vec);
    }
}

TEST(Bubble_sort_odd_even_test, reversed_sorted_vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int size_vector = 1000;
    std::vector<int> vec, ref_vec;

    if (rank == 0) {
        vec.resize(size_vector);
        for (int i = 0; i < size_vector; i++) {
            vec[i] = size_vector - 1 - i;
        }
        ref_vec = vec;
    }

    vec = BubbleSortOddEvenParallel(vec);

    if (rank == 0) {
        ref_vec = BubbleSortOddEvenSequential(ref_vec);
        ASSERT_EQ(ref_vec, vec);
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
