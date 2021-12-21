#include "mpi.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	int rank;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Status status;

	char *message;
	const int iterations = 1000000;
	const int N = 10;

	for (int array_size = 1; array_size <= iterations; array_size *= 10)
	{
		message = new char[array_size];

		double start = MPI_Wtime();

		for (int i = 0; i < N; i++) 
		{
			if (rank == 0) {
				MPI_Send(message, array_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
				MPI_Recv(message, array_size, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &status);
			}
			if (rank == 1) {
				MPI_Recv(message, array_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
				MPI_Send(message, array_size, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
			}
		}

		if (rank == 0)
		{
			double stop = MPI_Wtime();
			double time = stop - start;
			double bandwidth = 2. * N * array_size / time;
			double latency = time / (2. * N);
			cout << endl;
			cout << "Latency: " << latency << endl;
			cout << "Bandwidth for " << array_size << ": " << bandwidth << " bytes/s" << endl;
		}
	}
	
	MPI_Finalize();
	return 0;
}