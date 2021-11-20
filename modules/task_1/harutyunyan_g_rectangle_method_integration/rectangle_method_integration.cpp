// Copyright 2021 Harutyunyan Gurgen
#include <mpi.h>
#include <functional>
#include "../../../modules/task_1/harutyunyan_g_rectangle_method_integration/rectangle_method_integration.h"

double getSequentialOperations(double a, double b, int n, 
	const std::function<double(double)>& f)
{
	const double step = (b - a) / n;
	double rectangle_integration = 0;
	for (int i= 0; i < n; i++)
	{
		double x_1 = a + i * step;
		double x_2 = x_1 + step / 2;
		rectangle_integration += step * f(x_2);

	}
	
	return rectangle_integration;

}

double getParallelOperations(double a, double b, int n, 
	const std::function<double(double)>& f)
{
	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	double res_sum = 0;
	double sum;
	const double step = (b - a) / n;
	for (int i = 0; i < n; i++){
		double x_1 = a + i * step;
		double x_2 = x_1 + step / 2;
		res_sum += step * f(x_2);
	}
	MPI_Reduce(&res_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	return sum;
}
