//Reference to https://edoras.sdsu.edu/~mthomas/sp17.605/lectures/MPI-VectorOps.pdf

#include "mpi.h"
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <cstdlib>

using namespace std;

int dot_prod(int *a, int *b, int n)
{
	int s = 0;
	for (int i = 0; i < n; i++) {
		s += a[i] * b[i];
	}
	return s;
}


int main(int argc, char* argv[])
{
	int n = 1000000;
	int rank, size, batch;
	int* a = NULL;
	int* b = NULL;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	batch = n / size;

	if (rank == 0)
	{
		a = new int[n];
		b = new int[n];
		for (int i = 0; i < n; i++)
		{
			a[i] = 1;
			b[i] = 1;
		}
	}

	int* a1 = new int[batch];
	int* b1 = new int[batch];

	MPI_Scatter(a, batch, MPI_INT, a1, batch, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(b, batch, MPI_INT, b1, batch, MPI_INT, 0, MPI_COMM_WORLD);


	int dot = dot_prod(a1, b1, batch);
	int res = 0;

	MPI_Reduce(&dot, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) 
	{
		cout << "Result = " << res << endl;
	}
	
	MPI_Finalize();
	return 0;
}