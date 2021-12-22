#include <cstdlib>
#include <vector>
#include <iostream>
#include <algorithm>
#include "mpi.h"

using namespace std;


double random()
{
	return double(rand()) / double(RAND_MAX);
}

int main(int argc, char** argv)
{
	int size, rank;
	const int n = 500;
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double** A = new double*[n];
	double** B = new double*[n];
	double** C = new double*[n];

	for (int i = 0; i < n; i++)
	{
		A[i] = new double[n];
		B[i] = new double[n];
		C[i] = new double[n];
		for (int j = 0; j < n; j++)
		{
			A[i][j] = random();
			B[i][j] = random();
			C[i][j] = 0;
		}
	}

	MPI_Barrier(MPI_COMM_WORLD); //barrier process synchronization

	double start_time = MPI_Wtime();
	for (int i = 0; i < n; i++)
	{
		for (int k = 0; k < n; k++)
		{
			for (int j = 0; j < n; j++)
			{
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	double stop_time = MPI_Wtime();
	double time = stop_time - start_time;
	cout << "Process " << rank << ": " << time << " seconds" << endl;

	MPI_Finalize();
}