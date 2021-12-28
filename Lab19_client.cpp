#include <stdio.h>
#include <string.h>
#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char **argv)
{
	int got, send = 1;
	MPI_Init(&argc, &argv);
	char port_name[MPI_MAX_PORT_NAME];
	MPI_Status status;
	MPI_Comm intercomm;
	strcpy(port_name, argv[1]);
	cout << "Attempt to connect " << endl;
	MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
	cout << "Server connection " << endl;
	
	MPI_Send(&send, 1, MPI_INT, 0, 0, intercomm);
	cout << "Client sent value: " << send << endl;

	MPI_Recv(&got, 1, MPI_INT, 0, 0, intercomm, &status);
	cout << "Client received value: " << got << endl;

	MPI_Finalize();
	return 0;
}