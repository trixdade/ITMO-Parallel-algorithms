#include <stdio.h>
#include "mpi.h"
#include <iostream>

#define BUFSIZE 100

using namespace std;
int main(int argc, char** argv)
{
	int bufsize, num = 0, sum;
	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_File fh;
	char buf[9];
	const char* filename = "file.txt";
	string text = "hey man!";
	int len = text.length();
	const char* cstr = text.c_str();
	MPI_File_open(MPI_COMM_WORLD, filename, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
	MPI_File_write(fh, cstr, len, MPI_CHAR, &status);
	MPI_File_close(&fh);
	MPI_File_open(MPI_COMM_WORLD, filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
	MPI_File_set_view(fh, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
	sum = 0;
	do {
		MPI_File_read(fh, buf, BUFSIZE, MPI_CHAR, &status);
		MPI_Get_count(&status, MPI_CHAR, &num);
		printf("buf=%.8s\n", buf);
		sum += num;
	} while (num >= BUFSIZE);
	MPI_File_close(&fh);
	cout << "number of symbols: " << sum << endl;
	char output;
	cout << "delete file?(y/n)" << endl;
	cin >> output;
	if (output == 'y')
	{
		remove("file.txt");
		cout << "file is deleted" << endl;
	}
	else
	{
		cout << "file isn't deleted" << endl;
	}

	MPI_Finalize();
}