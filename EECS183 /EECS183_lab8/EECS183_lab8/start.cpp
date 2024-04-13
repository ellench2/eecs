/**
 * start.cpp
 *
 * EECS 183
 * Lab 10: Exam 2 Practice
 *
 */

#include <iostream>
#include <string>

using namespace std;

//********************************************************
// Function declarations. 
// Function definitions are in test.cpp and monsters.cpp
//********************************************************
void startTests();
void monsters();

int main() {
    
    cout << "--------------------------------------------" << endl
         << "EECS 183 Lab 10 Menu Options" << endl
         << "--------------------------------------------" << endl;
    cout << "1) Execute testing functions in test.cpp" << endl; 
    cout << "2) Execute monsters function in monsters.cpp" << endl;    
    cout << "Choice --> ";

    int choice;
    cin >> choice;

    // remove the return character from the cin stream buffer
    string junk;
    getline(cin, junk);

    if (choice == 1) {
        startTests();
    }
    else if (choice == 2) {
        monsters();
    }
    
    return 0;
}
