#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <omp.h>
#include <chrono>

using namespace std;

int main(int argc, char *argv[])
{
    int N = atoi(argv[1]);
    srand(42);
    vector<int> v(N);
    generate(v.begin(), v.end(), std::rand);

    int i = 0;
    /*for (auto const& el : v)
    {
        cout << "v[" << i << "]: " << el << endl;
        i++;
    }*/

    vector<std::chrono::microseconds> time;
    int max_el;

    for (int num=1; num<=10; num++)
    {
        max_el = 0;
        auto t1 = chrono::high_resolution_clock::now();
        #pragma omp parallel for num_threads(num) reduction(max : max_el) 

        for (int i = 0; i < N; i++) {
            max_el = max_el > v[i] ? max_el : v[i];
        }
        auto t2 = chrono::high_resolution_clock::now(); 
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1);
        time.push_back(duration);
    }
    
        
    cout << "Max: " << max_el << endl;
    
    i = 1;
    for (auto const& el : time)
    {
        cout << "With " << i << " thread: " << el.count() << endl;
        i++;
    }

    return 0;
}