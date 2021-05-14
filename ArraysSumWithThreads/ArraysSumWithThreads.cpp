#include <iostream>
#include <random>
#include <ctime>
#include <thread>
#include <chrono>

using namespace std;


void print_arr(const int* arr, const int size) {
    for (int i = 0; i < size; ++i)
        cout << arr[i] << ", ";
    cout << endl;
}


void fill_arr_randomly(int* arr, const int size) {
    srand(time(NULL));
    for (int i = 0; i < size; ++i)
        arr[i] = rand() % 10;
}


void sum_arr(const int* arr, const int size, int& sum) {
    sum = 0;
    for (int i = 0; i < size; ++i)
        sum += arr[i];
}


void sum_arr_partial(const int* arr, const int size, const int begin, const int end, int& partial_sum) {
    if (begin > size || end > size || begin < 0 || end < 0 || begin > end)
        return;

    partial_sum = 0;
    
    for (int i = begin; i < end; ++i)
        partial_sum += arr[i];
}

int main()
{
    int size = 200000000;
    int* arr = new int[size];
    int number_of_threads = 2;

    auto begin = chrono::high_resolution_clock::now();
    
    thread t_fill_arr(fill_arr_randomly, arr, size);
    if (t_fill_arr.joinable())
        t_fill_arr.join();

    /*thread t_print_arr(print_arr, arr, size);
    if (t_print_arr.joinable())
        t_print_arr.join();*/

    int partial_sum1 = 0;
    thread t_partial_sum1(sum_arr_partial, arr, size, 0, size / number_of_threads, ref(partial_sum1));
    
    int partial_sum2 = 0;
    thread t_partial_sum2(sum_arr_partial, arr, size, size / number_of_threads, size, ref(partial_sum2));

    if (t_partial_sum1.joinable())
        t_partial_sum1.join();
    cout << "1 partial sum  = " << partial_sum1 << endl;

    if (t_partial_sum2.joinable())
        t_partial_sum2.join();
    cout << "2 partial sum  = " << partial_sum2 << endl;

    int total_sum = partial_sum1 + partial_sum2;
    cout << "sum of array's elements = " << total_sum << endl;

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<float> duration = end - begin;
    cout << "duraion = " << chrono::duration_cast<chrono::milliseconds>(duration).count() << endl;




    return 0;
}


