//
//  main.cpp
//  test_2
//
//  Created by Ellen Chlachidze on 10/14/22.
//

#include <iostream>
int partition(int a[], int left, int right) {
    int pivot = --right;
    while (true) {
        while (a[left] < a[pivot])
            ++left;
        while (left < right && a[right - 1] >= a[pivot])
            --right;
        if (left >= right)
            break;
        std::swap(a[left], a[right - 1]);
    }
    std::swap(a[left], a[pivot]);
    return left;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    int arr[] = {88, 34, 77, 20, 53, 45, 12, 76, 29, 61};
    partition(arr, 0, 10);
    
    return 0;
}
