#include <stdio.h>
#include "mpi.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, n, i, message;
    double time_start, time_finish;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    time_start = MPI_Wtime();
    if (rank == 3)
    {
        cout << "Hello from process " << rank << "\n";
        for (i = 1; i < n; i++) {
            MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE,
                     MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            cout << "Hello from process " << message << endl;
        }
    }
    else MPI_Send(&rank, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
    time_finish = MPI_Wtime();
    cout << "process " << rank << " time = " << (time_finish - time_start) << endl;
    MPI_Finalize();
    return 0;
}