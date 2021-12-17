// Copyright 2021 Ermakov Pavel
#include <gtest/gtest.h>
#include <mpi.h>
#include <cstdlib>
#include <random>
#include <vector>
#include <ctime>
#include <gtest-mpi-listener.hpp>

#ifndef MODULES_TASK_2_ERMAKOV_P_CONTRAST_ENHANCEMENT_CONTRAST_ENHANCEMENT_H_
#define MODULES_TASK_2_ERMAKOV_P_CONTRAST_ENHANCEMENT_CONTRAST_ENHANCEMENT_H_

std::vector<std::vector<int>> pic_gen(int h, int w);
int barrier(int pixel);
std::vector<std::vector<int>> contrast_enhancement_seq(std::vector<std::vector<int>> pic, double alpha, int beta);
std::vector<int> contrast_enhancement_par(std::vector<int> tmp, double alpha, int beta, int rows, int cols);

#endif  // MODULES_TASK_2_ERMAKOV_P_CONTRAST_ENHANCEMENT_CONTRAST_ENHANCEMENT_H_
