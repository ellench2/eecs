/**
 * arrays.cpp
 *
 * EECS 183
 * Lab 7: Array exercises
 *
 * Ellen Chlachidze
 * ellench
 *
 * <#A description of the assignment here#>
 */

#include "arrays.h"
#include <iostream>

using namespace std;

void print_array(const int array[], int size) {
    cout << "{" << array[0];
    for (int i = 1; i < size; i++) {
        cout << ", " << array[i];
    }
    cout << "}" << endl;
    return;
}

void negate_all(int arr[], int size);
bool is_sorted(const int arr[], int size);
void copy_positive_elements(const int source[], int target[], int size);
void sort_array(int arr[], int size);

void negate_all(int arr[], int size){
    for (int i = 0; i < size; i++){
        arr[i] = -arr[i];
    }
}

bool is_sorted(const int arr[], int size){
    bool sorted = true;
    int a = 0;
    for (int i = 0; i < size - 1; i++){
        if (arr[i] > arr[i+1]){
            a++;
        }
    }
    if (a > 0){
        sorted = false;
    }
    return sorted;
}

void copy_positive_elements(const int source[], int target[], int size){
    for (int i = 0; i < size; i++){
        target[i] = 0;
    }
    int a = 0;
    for (int i = 0; i < size; i++){
        if (source[i] > 0) {
            target[a] = source[i];
            a++;
        }
    }
}

void sort_array(int arr[], int size){
    while (!is_sorted(arr, size)){
        for (int i = 0; i < size - 1; i++){
            if (arr[i] > arr[i+1]) {
                int temp = arr[i+1];
                arr[i+1] = arr[i];
                arr[i] = temp;
            }
        }
    }
}
