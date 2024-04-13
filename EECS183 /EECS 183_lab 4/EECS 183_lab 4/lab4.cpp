/**
 * lab3.cpp
 *
 * Ellen Chlachidze
 * ellench
 *
 * EECS 183: Lab 4
 *
 * <#description#>
 */

#include <iostream>
using namespace std;

/**
 * Note: You will not implement this function as part of the lab
 *
 * Requires: month, day, year may represent a date
 * Modifies: nothing
 * Effects: returns 'true' if the date is in the limits
 *          of the Gregorian calendar otherwise returns 'false'
 */
bool isGregorianDate(int month, int day, int year);

/**
 * Note: You will not implement this function as part of the lab
 *
 * Requires: year is a Gregorian year
 * Modifies: nothing
 * Effects: Returns 'true' if the year is a leap year
 *          otherwise returns 'false'
 */
bool isLeapYear(int year);

void test_isGregorianDate();
void test_isLeapYear();

int main()
{
    test_isLeapYear();
    cout << endl;
    test_isGregorianDate();

    return 0;
}

void test_isGregorianDate()
{
    cout << "Testing -- isGregorianDate()" << endl;
    cout << "January 1st, 1999: Expected 1, actual " << isGregorianDate(1, 1, 1999) << endl;
    cout << "September 14th, 1752: Expected 1, actual " << isGregorianDate(9, 14, 1752) << endl;
    cout << "September 13th, 1752: Expected 0, actual " << isGregorianDate(9, 13,1752) << endl;
    cout << "March 12th, 1000: Expected 0, actual " << isGregorianDate(3, 12, 1000) << endl;
    

    cout << "Finished testing -- isGregorianDate()" << endl
         << endl;
}

void test_isLeapYear()
{
    cout << "Testing -- isLeapYear()" << endl;
    cout << "2000: Expected 1, actual " << isLeapYear(2000) << endl;
    cout << "1900: Expected 0, actual " << isLeapYear(1900) << endl;
    cout << "2004: Expected 1, actual " << isLeapYear(2004) << endl;
    cout << "2001: Expected 0, actual " << isLeapYear(2001) << endl;
    cout << "Finished testing -- isLeapYear()" << endl;
}

// DO NOT ALTER THE CODE BELOW THIS LINE IN ANY WAY
// WRITE YOUR CODE IN THE FUNCTIONS ABOVE

#if defined(DEBUG) || defined(_DEBUG)
bool isGregorianDate(int month, int day, int year)
{
    // this (incorrect) implementation returns false to avoid compile error
    // you DO NOT need to implement this function as part of the lab
    return false;
}

bool isLeapYear(int year)
{
    // this (incorrect) implementation returns false to avoid compile error
    // you DO NOT need to implement this function as part of the lab
    return false;
}
#endif
