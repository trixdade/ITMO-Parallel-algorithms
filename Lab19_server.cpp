#include <stdio.h>
#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char **argv)
{
	int got, send = 2;
	MPI_Init(&argc, &argv);
	char port_name[MPI_MAX_PORT_NAME];
	MPI_Status status;
	MPI_Comm intercomm;
	MPI_Open_port(MPI_INFO_NULL, port_name);
	cout << "Waiting for the client: " << endl;
	MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
	cout << "Client connected " << endl;
	MPI_Recv(&got, 1, MPI_INT, 0, 0, intercomm, &status);
	MPI_Send(&send, 1, MPI_INT, 0, 0, intercomm);
	MPI_Comm_free(&intercomm);
	MPI_Close_port(port_name);
	cout << "Server sent value: " << send << endl;
	cout << "Server received value: " << got << endl;
	MPI_Finalize();
	return 0;
}