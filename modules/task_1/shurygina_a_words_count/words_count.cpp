// Copyright 2021 Shurygina A
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <iostream>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_1/shurygina_a_words_count/words_count.h"

int SimpleCount(const std::string str) {
    if (str.empty()) {
        return 0;
    }
    int s = str.size();
    int i;
    int count = 0;
    for (i = 0; i < s; i++) {
        if ((str[i] == ' ') && (str[i + 1] != ' ')) count++;
        if ((i == 0) && str[i] == ' ') count--;
        if ((i == s - 1) && str[i] == ' ') count--;
    }
    return count + 1;
}

int ParallelCount(const std::string inputString) {
    int size, rank;
    // Number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Rank of a process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (inputString.empty()) {
        return 0;
    }
    int integerPart;
    int remainder;
    integerPart = inputString.size() / size;
    remainder = inputString.size() % size;
    // Distribution of work between processes by 0 process
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            int start = 0;
            int end = 0;
            start = i * integerPart + remainder;
            end = start + integerPart;
            MPI_Send(&start, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&end, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
        }
    }
    std::string letters = "";
    int start, end;
    if (rank == 0) {
        letters = inputString.substr(0, integerPart + remainder);
    } else {
        // Result of data transition
        MPI_Status status;
        // Fixing the order of receiving messages
        MPI_Recv(&start, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&end, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
        for (int i = start; i < end; i++)
            letters += inputString[i];
    }
    int resultCount = 0;
    int deltaCountProcesses;
    if (rank == 0) {
        deltaCountProcesses = SimpleCount(letters);
    } else {
        deltaCountProcesses = DeltaProcessCount(letters);
    }
    if ((rank != size - 1) && (letters[letters.size() - 1] == ' ')) {
        deltaCountProcesses++;
    }
    // Reduction of data of all processes
    MPI_Reduce(&deltaCountProcesses, &resultCount,
        1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return resultCount;
}

int DeltaProcessCount(const std::string str) {
    if (str.empty()) {
        throw EMPTY_STRING_ERROR;
    }
    int s = str.size();
    int i;
    int count = 0;
    for (i = 0; i < s; i++) {
        if ((str[i] == ' ') && (str[i + 1] != ' ')) count++;
        if ((i == s - 1) && str[i] == ' ') count--;
    }
    return count;
}

std::string getLongString(int size) {
    std::string result = "";
    for (int i = 0; i < size; i++) {
        result += "Anna ";
    }
    return result;
}
