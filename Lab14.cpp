#include <stdio.h>
#include <iostream>
#include "mpi.h"
#define n 1000

void smod5(void* a, void* b, int* l, MPI_Datatype* type) {
	for (int i = 0; i < *l; i++)
		((int*)b)[i] = (((int*)a)[i] + ((int*)b)[i]) % 5;
}


void maximum(void* a, void* b, int* l, MPI_Datatype* type)
{
	for (int i = 0; i < *l; i++)
		if (((int*)a)[i] > ((int*)b)[i]) ((int*)b)[i] = ((int*)a)[i];
}

int main(int argc, char** argv)
{
	int rank, size;
	int a[n];
	int b[n];
	int b_test[n];

	MPI_Init(&argc, &argv);
	MPI_Op op;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// fill a[n]
	for (int i = 0; i < n; i++) a[i] = i + rank + 1;

	
	MPI_Op_create(&maximum, 1, &op);
	MPI_Reduce(a, b, n, MPI_INT, op, 0, MPI_COMM_WORLD);
	MPI_Op_free(&op);

	if (rank == 0) 
	{
		std::cout << "b[0] = " << b[0] << std::endl;
	}


	MPI_Reduce(a, b_test, n, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	
	if (rank == 0) 
	{
		std::cout << "b_test[0] = " << b_test[0] << std::endl;
	}

	MPI_Finalize();
}