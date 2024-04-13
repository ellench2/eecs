/**
 * lab2.cpp
 * 
 * Ellen Chlachidze
 * ellench
 * 
 * EECS 183:Lab 2
 *
 * Puppy adoption program (formerly Hello World)
 */

#include <iostream>
using namespace std;

int main() {
    int pugsAdopted;
    cout << "Pug Adoption Program" << endl;
    cout << "How many Pugs will be adopted today?" << endl;
    cin >> pugsAdopted;
    int pugsRemaining = 20 - pugsAdopted;
    cout << "Pugs remaining: " << pugsRemaining << endl;
    return 0;
}

