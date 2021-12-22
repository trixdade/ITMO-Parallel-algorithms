#include "mpi.h"
#include <iostream>

using namespace std;

int v_sum(int* v, int n)
{
	int s = 0;
	for (int i = 0; i < n; i++) {
		s += v[i];
	}
	return s;
}

int parallel_sum_doubling(int *x, int batch_size, int rank, int size, MPI_Status status)
{
	int sum, receiver, i, leaf;
	sum = v_sum(x, batch_size);
	for (int proc = 2; proc <= size; proc *= 2)
	{
		i = rank % proc;
		leaf = rank + proc / 2;
		if (leaf < size & !i)
		{
			MPI_Recv(&receiver, 1, MPI_INT, leaf, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			sum += receiver;
		}
		else
		{
			MPI_Send(&sum, 1, MPI_INT, rank - i, 0, MPI_COMM_WORLD);
		}
	}
	return sum;
}

int main(int argc, char* argv[])
{
	int size, rank;
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Status status = {};

	const int n = 1000000;

	int* v = NULL;

	if (rank == 0)
	{
		v = new int[n];
		for (int i = 0; i < n; i++) v[i] = 1;
	}

	int batch_size = n / size;

	int* v_sub = new int[batch_size];

	MPI_Scatter(v, batch_size, MPI_INT, v_sub, batch_size, MPI_INT, 0, MPI_COMM_WORLD);

	int loc_sum = 0;
	for (int i = 0; i < batch_size; i++)
	{
		loc_sum += v_sub[i];
	}
	int glob_sum = 0;

	double start_reduce = MPI_Wtime();
	MPI_Reduce(&loc_sum, &glob_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	double stop_reduce = MPI_Wtime();
	double time_reduce = stop_reduce - start_reduce;

	if (rank == 0)
	{
		cout << "Reduce time: " << time_reduce << endl;
	}

	double start_cascade = MPI_Wtime();
	int cascade_sum = parallel_sum_doubling(v_sub, batch_size, rank, size, status);
	double stop_cascade = MPI_Wtime();
	double time_cascade = stop_cascade - start_cascade;

	if (rank == 0)
	{
		cout << "Cascade time: " << time_cascade << endl;
	}

	if (rank == 0)
	{
		cout << "Reduction sum = " << glob_sum << endl;
		cout << "Cascade sum = " << cascade_sum << endl;
	}
	
	MPI_Finalize();
	return 0;
}