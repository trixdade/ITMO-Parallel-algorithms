#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <omp.h>
#include <chrono>

using namespace std;

using Matrix = vector<vector<double>>;

double random();
Matrix fill_matrix(int n);
void show_matrix(Matrix A, int n);

Matrix multiply_ijk(Matrix A, Matrix B, int n);
Matrix multiply_ikj(Matrix A, Matrix B, int n);
Matrix multiply_jki(Matrix A, Matrix B, int n);
Matrix multiply_jik(Matrix A, Matrix B, int n);

Matrix multiply_ikj_time(Matrix A, Matrix B, int n);

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);
    Matrix A(n, vector<double>(n));
    Matrix B(n, vector<double>(n)); 

    A = fill_matrix(n);
    B = fill_matrix(n);
    
    //cout << "Matrix A:" << endl;
    //show_matrix(A, n);
    
    //cout << "Matrix B:" << endl;
    //show_matrix(B, n);

    Matrix C(n, vector<double>(n));
    //C = multiply_ijk(A, B, n);
    
    //cout << "Matrix C:" << endl;
    //show_matrix(C, n);



    
    C = multiply_ikj_time(A, B, n);

    

    return 0;
}

    
Matrix multiply_ikj_time(Matrix A, Matrix B, int n)
{
    Matrix C(n, vector<double>(n));
    clock_t t;
    double first;

    for (int num=1; num<=10; num++)
    {
        t = clock();

        #pragma omp parallel for num_threads(num)
        for (int i=0; i<n; i++)
        {
            for (int k=0; k<n; k++)
                {
                    for (int j=0; j<n; j++)
                    {
                        C[i][j] += A[i][k] * B[k][j];
                    }
                }    
        }
        t = clock() - t;
        double tim = double(t) / CLOCKS_PER_SEC;

        if (num == 1) 
        {
            first = tim;
        }

        cout << num << " thread: " << tim << endl;
        cout << "Efficiency: " << first / tim << endl; 
        cout << endl;
    }
    return C;
}

Matrix multiply_ijk(Matrix A, Matrix B, int n)
{
    Matrix C(n, vector<double>(n));
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
            {
                for (int k=0; k<n; k++)
                {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }    
    }
    return C;
}

Matrix multiply_jik(Matrix A, Matrix B, int n)
{
    Matrix C(n, vector<double>(n));
    for (int j=0; j<n; j++)
    {
        for (int i=0; i<n; i++)
            {
                for (int k=0; k<n; k++)
                {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }    
    }
    return C;
}

Matrix multiply_ikj(Matrix A, Matrix B, int n)
{
    Matrix C(n, vector<double>(n));
    for (int i=0; i<n; i++)
    {
        for (int k=0; k<n; k++)
            {
                for (int j=0; j<n; j++)
                {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }    
    }
    return C;
}

Matrix multiply_jki(Matrix A, Matrix B, int n)
{
    Matrix C(n, vector<double>(n));
    for (int j=0; j<n; j++)
    {
        for (int k=0; k<n; k++)
            {
                for (int i=0; i<n; i++)
                {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }    
    }
    return C;    
}


double random()
{ 
    return double(rand())/double(RAND_MAX);
}

void show_matrix(Matrix A, int n)
{
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
        {
            cout.width(10);
            cout.precision(3);
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
}

Matrix fill_matrix(int n)
{
    Matrix A(n, vector<double>(n));
    for (int i=0; i<n; i++)
    {
        generate(A[i].begin(), A[i].end(), random);
    }
    return A;
}