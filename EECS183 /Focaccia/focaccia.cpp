/**
 * focaccia.cpp
 *
 * Ellen Chlachidze
 * ellench
 *
 * EECS 183: Project 1
 * Fall 2021
 * 
 * Project UID: fde244392017fe65ebcc34f01c226b11f113bb3dc6dae8af4cb6ea11bf76f7c8
 */

#include <iostream>
#include <cmath>
#include <string>
using namespace std;

/**
 * Returns the singular or plural form of a word, based on number
 *
 * Requires: singular is the singular form of the word.
 *           plural is the plural form of the word.
 *           number determines how many things there are; must be >= 0.
 * Modifies: Nothing.
 * Effects:  Returns return the singular form of the word if number == 1.
 *           Otherwise, returns the plural form.
 * Examples:
 *           // prints "bag"
 *           cout << pluralize("bag", "bags", 1);
 *
 *           // prints "pounds"
 *           string temp = pluralize("pound", "pounds", 3);
 *           cout << temp;
 */
string pluralize(string singular, string plural, int number);



int main() {
    int people;
    cout << "How many people do you need to serve? ";
    cin >> people;
    cout << endl << endl;

    int flourbags;
    int packages;
    int canisters;
    int bottles;
    double totalCost;
    int loaves = static_cast<int>(ceil(people / 4.0));
    if (loaves > 1 || loaves == 0) {cout << "You need to make: " << loaves << " loaves of focaccia" << endl << endl;};
    if (loaves == 1) {cout << "You need to make: " << loaves << " loaf of focaccia" << endl << endl;};
    cout << "Shopping List for Focaccia Bread" << endl << "--------------------------------" << endl;
    flourbags = static_cast<int>(ceil(loaves / 4.0));
    if (flourbags > 1 || flourbags == 0) {cout << flourbags << " bags of flour" << endl; };
    if (flourbags == 1) {cout << flourbags << " bag of flour" << endl; };
    packages = static_cast<int>(ceil(loaves * 7 / 9.0));
    if (packages > 1 || packages == 0) {cout << packages << " packages of yeast" << endl; };
    if (packages == 1) {cout << packages << " package of yeast" << endl; };
    canisters = static_cast<int>(ceil(loaves * 15 / 48.0));
    if (canisters > 1 || canisters == 0) {cout << canisters << " canisters of salt" << endl; };
    if (canisters == 1) {cout << canisters << " canister of salt" << endl; };
    bottles = static_cast<int>(ceil(loaves * 74 / 1250.0));
    if (bottles > 1 || bottles == 0) {cout << bottles << " bottles of olive oil" << endl << endl; };
    if (bottles == 1) {cout << bottles << " bottle of olive oil" << endl << endl; };
    totalCost = flourbags * 2.69 + packages * 0.40 + canisters * 0.49 + bottles * 6.39;
    cout << "Total expected cost of ingredients: $" << totalCost << endl << endl;
    cout << "Have a great party!" << endl;
    return 0;
}

// ----------------------------------------------
// *** DO NOT CHANGE ANYTHING BELOW THIS LINE ***

string pluralize(string singular, string plural, int number) {
    if (number == 1) {
        return singular;
    }
    return plural;
}
