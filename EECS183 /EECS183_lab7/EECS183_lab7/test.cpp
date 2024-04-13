/**
 * test.cpp
 *
 * EECS 183
 * Lab 7
 *
 * <#Name#>
 * <#uniqname#>
 *
 * <#Description#>
 */

#include <iostream>
#include <fstream>
using namespace std;

#include "Point.h"

void test_Point();

int main() {
    test_Point();

    return 0;
}

void test_Point() {
    Point pt1;
    pt1.setX(15);
    cout << "Expected: (15,0), actual :" << pt1 << endl;
        
    ifstream input_file;
    input_file.open("data1.txt");
    pt1.read(input_file);
    cout << "Expected: (42,7), actual :" << pt1 << endl;
    pt1.setX(100);
    cout << "Expected: (100,7), actual: " << pt1 << endl;
    cout << "Expected: 99, actual: " << pt1.getX() << endl;
    pt1.setY(12);
    cout << "Expected: 12, actual: " << pt1.getY() << endl;
    Point pt2(3, 4);
    cout << "Expected: 3, actual: " <<  pt2.getX() << endl;
    cout << "Expected: 4, actual: " << pt2.getY() << endl;
    Point pt3(130, -42);
    cout << "Expected: 99, actual: " << pt3.getX() << endl;
    cout << "Expected: 0, actual: " << pt3.getY() << endl;
    return;
}
