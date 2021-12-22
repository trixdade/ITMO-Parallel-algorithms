#include <iostream>
#include "mpi.h"
using namespace std;

int main(int argc, char** argv) {
	int rank, size, prev, next;
	
	float rbuf[2], sbuf[2];

	MPI_Request reqs[4];
	MPI_Status stats[4];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int test = 0;
	
	prev = rank - 1;
	next = rank + 1;
	if (rank == 0) prev = size - 1;
	if (rank == size - 1) next = 0;
	
	MPI_Recv_init(&rbuf[0], 1, MPI_FLOAT, prev, 5, MPI_COMM_WORLD, &reqs[0]);
	MPI_Recv_init(&rbuf[1], 1, MPI_FLOAT, next, 6, MPI_COMM_WORLD, &reqs[1]);
	MPI_Send_init(&sbuf[0], 1, MPI_FLOAT, prev, 6, MPI_COMM_WORLD, &reqs[2]);
	MPI_Send_init(&sbuf[1], 1, MPI_FLOAT, next, 5, MPI_COMM_WORLD, &reqs[3]);
	
	sbuf[0] = float(rank);
	sbuf[1] = float(rank);
	MPI_Startall(4, reqs);
	MPI_Waitall(4, reqs, stats);

	MPI_Testall(4, reqs, &test, stats);

	if (test)
	{
		cout << rbuf[0]  << " send to " << rank << " which send to " << rbuf[1] << endl;
	}

	MPI_Request_free(&reqs[0]);
	MPI_Request_free(&reqs[1]);
	MPI_Request_free(&reqs[2]);
	MPI_Request_free(&reqs[3]);
	MPI_Finalize();
	return 0;
}