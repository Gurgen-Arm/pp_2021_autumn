// Copyright 2018 Nesterov Alexander
#ifndef MODULES_TASK_1_HARUTYUNYAN_G_RECTANGLE_METHOD_INTEGRATION_RECTANGLE_METHOD_INTEGRATION_H_
#define MODULES_TASK_1_HARUTYUNYAN_G_RECTANGLE_METHOD_INTEGRATION_RECTANGLE_METHOD_INTEGRATION_H_


#include <functional>

double getSequentialOperations(double a, double b, int n, const std::function<double(double)>& f);
double getParallelOperations(double a, double b, int n, const std::function<double(double)>& f);
#endif  // MODULES_TASK_1_HARUTYUNYAN_G_RECTANGLE_METHOD_INTEGRATION_RECTANGLE_METHOD_INTEGRATION_H_
