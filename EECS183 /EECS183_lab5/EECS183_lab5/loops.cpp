/**
 * loops.cpp
 *
 * EECS 183
 * Lab 5: Loops exercises
 *
 * Ellen Chlachidze
 * ellench
 *
 * <#A description of the assignment here#>
 */

#include "loops.h"
#include <iostream>

using namespace std;

/** 
 * TODO
 * Implement all loops.h functions:
 * printRectangle, printRight, printRightJustified, printIsosceles
 */

void printRectangle(int rows, int cols) {
    for (int i = rows; i > 0; i--) {
        for (int j = cols; j > 0; j--) {
            cout << "*";
        }
        cout << endl;
    }
}

void printRight(int n) {
    for (int i = 1; i < n + 1; i++) {
        for (int j = i; j > 0; j--) {
            cout << "*";
        }
        cout << endl;
    }
}

void printRightJustified(int n) {
    for (int i = 1; i < n + 1; i++) {
        for (int j = n - i; j > 0; j--) {
            cout << " ";
        }
        for (int k = 0; k < i; k++) {
            cout << "*";
        }
        cout << endl;
    }
}

void printIsosceles(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i - 1; j < n - 1; j++) {
            cout << " ";
        }
        for (int k = -1; k < 2*i; k++){
            cout << "*";
        }
        for (int j = i - 1; j < n - 1; j++) {
            cout << " ";
        }
        cout << endl;
    }
}
