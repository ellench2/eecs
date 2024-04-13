/**
 * test.cpp
 *
 * EECS 183
 * Lab 5: loops exercises
 *
 * <#Name(s)#>
 * <#uniqname(s)#>
 *
 * <#A description of the assignment here#>
 */

#include "loops.h"

#include <iostream>
using namespace std;

void testPrintRectangle();
void testPrintRight();
void testPrintRightJustified();
void testPrintIsosceles();
int main() {
    
    // TODO
    // Once you have completed the testing functions
    // call them such that the tests actually get run
    testPrintRectangle();
    testPrintRight();
    testPrintRightJustified();
    testPrintIsosceles();
    return 0;
}

void testPrintRectangle() {
    cout << "Testing printRectangle()" << endl;
    printRectangle(3, 1);
    printRectangle(1, 4);
    
}

// TODO
// Implement your other test_functions

void testPrintRight() {
    cout << "Testing printRight()" << endl;
    printRight(4);
    printRight(5);
}

void testPrintRightJustified() {
    cout << "Testing printRightJustified()" << endl;
    printRightJustified(4);
    printRightJustified(6);
}

void testPrintIsosceles() {
    cout << "Testing printIsosceles()" << endl;
    printIsosceles(4);
    printIsosceles(5);
}

