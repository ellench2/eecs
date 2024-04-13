/**
 * test.cpp
 * Project UID: d27ac2f9d9ef51141f8207ca61bd9b22d96076f1
 * 
 * <#Name#>
 * <#Uniqname#>
 *
 * EECS 183: Project 2
 *
 * Testing functions for your birthdays.cpp implementation.
 * Holds the definitions of required testing functions.
 * We have stubbed all required functions for you.
 */

#include <cassert>
#include <cctype>
#include <iostream>
#include <limits>
#include <string>

using namespace std;


//************************************************************************
// You should have implemented the following functions in birthdays.cpp
//************************************************************************
int getMenuChoice();
bool isLeapYear (int year);
bool isGregorianDate(int m, int d, int y);
bool isValidDate(int month, int day, int year);
int determineDay (int month, int day, int year);
void printDayOfBirth(int day);
void determineDayOfBirth();
void print10LeapYears();

//************************************************************************
// Testing function declarations. Function definition is below main.
// Tests not run by these functions will not be run by the autograder.
//************************************************************************
void test_getMenuChoice();
void test_isGregorianDate();
void test_isLeapYear();
void test_isValidDate();
void test_determineDay();
void test_printDayOfBirth();
void test_determineDayOfBirth();
void test_print10LeapYears();

void startTests()
{
    // Write calls to your test functions here, e. g.,
    test_getMenuChoice();
    test_isGregorianDate();
    test_getMenuChoice();
    test_isLeapYear();
    test_isValidDate();
    test_printDayOfBirth();
    test_determineDay();
    // Add more calls as you implement more functions.
    // While you're working on a function, it can be useful to comment
    //  out the calls to other functions' tests.

    // Make sure the only function you test in a test function
    //  like test_isGregorianDate is that function (isGregorianDate),
    //  or it may cause issues with the autograder.
}

void test_getMenuChoice()
{
    // getMenuChoice reads from cin, so you will need to
    // provide directions to the user for each test case
    cout << "Now testing funtion getMenuChoice()\n";
    cout << "Please enter 3:" << endl;
    int choice1 = getMenuChoice();
    cout << "Test result: expected: 3, actual: " << choice1 << endl;

    cout << "Please enter 5 then 1:";
    int choice2 = getMenuChoice();
    cout << "Test result: " << endl;
    cout << "Expected: \"Invalid menu choice\", then 1." << endl;
    cout << "Actual: " << choice2 << endl;

    cout << "Please enter 2 then 1:";
    int choice3 = getMenuChoice();
    cout << "Test result: " << endl;
    cout << "Expected: 2, then 1." << endl;
    cout << "Actual: " << choice3 << endl;
    
    cout << "Please enter 6 then 1:";
    int choice4 = getMenuChoice();
    cout << "Test result: " << endl;
    cout << "Expected: \"Invalid menu choice\", then 1." << endl;
    cout << "Actual: " << choice4 << endl;
    
    cout << "Please enter 7 then 1:";
    int choice5 = getMenuChoice();
    cout << "Test result: " << endl;
    cout << "Expected: \"Invalid menu choice\", then 1." << endl;
    cout << "Actual: " << choice5 << endl;
}

void test_isGregorianDate()
{
    // isGregorianDate does not read from cin, so
    // all its tests can be run automatically
    cout << "Now testing function isGregorianDate()\n";
    cout << "9/2/2019:  Expected: 1, Actual: " 
         << isGregorianDate(9, 2, 2019) << endl;
    cout << "1/31/1001: Expected: 0, Actual: "
         << isGregorianDate(1, 31, 1001) << endl;
    cout << "9/13/1752: Expected: 0, Actual: "
         << isGregorianDate(9, 13, 1752) << endl;
    cout << "9/14/1752: Expected: 0, Actual: "
         << isGregorianDate(9, 14, 1752) << endl;
}

void test_isLeapYear()
{
    cout << "Now testing function isLeapYear()" << endl;
    cout << "2016: Expected: 1, Actual: " << isLeapYear(2016) << endl;
    cout << "2000: Expected: 1, Actual: " << isLeapYear(2000) << endl;
    cout << "1900: Expected: 0, Actual: " << isLeapYear(1900) << endl;
    cout << "2001: Expected: 0, Actual: " << isLeapYear(2001) << endl;
    cout << "2014: Expected: 0, Actual: " << isLeapYear(2014) << endl;
}

void test_isValidDate()
{
    cout << "Now testing isValidDate()" << endl;
    cout << "15/4/2000: Expected: 0, Actual: " << isValidDate(15, 4, 2000) << endl;
    cout << "0/0/2000: Expected: 0, Actual: " << isValidDate(0, 0, 2000) << endl;
    cout << "9/4/1700: Expected: 0, Actual: " << isValidDate(9, 4, 1700) << endl;
    cout << "11/31/2000: Expected: 0, Actual: " << isValidDate(11, 31, 2000) << endl;
    cout << "2/29/2000: Expected: 1, Actual: " << isValidDate(2, 29, 2000) << endl;
    
}

void test_determineDay()
{
    cout << "Now testing determinDay()" << endl;
    cout << "9/29/2021: Expected: 4, Actual: " << determineDay(9, 29, 2021) << endl;
    cout << "12/03/2002: Expected: 3, Actual: " << determineDay(12, 3, 2002) << endl;
    cout << "2/29/2000: Expected: 3, Actual: " << determineDay(2, 29, 2002) << endl;
    cout << "1/01/1800: Expected: 4, Actual: " << determineDay(1, 1, 1800) << endl;
    cout << "9/28/2021: Expected: 3, Actual: " << determineDay(9, 28, 2021) << endl;
}

void test_printDayOfBirth()
{
    // printDayOfBirth prints to cout rather than
    // returning a value.
    cout << "Now testing function printDayOfBirth()\n";
    cout << "1: Expected: Sunday, Actual: ";
    printDayOfBirth(1);
    cout << endl;
    cout << "0: Expected: Saturday, Actual: ";
    printDayOfBirth(0);
    cout << endl;
    cout << "2: Expected: Monday, Actual: ";
    printDayOfBirth(2);
    cout << endl;
    cout << "3: Expected: Tuesday, Actual: ";
    printDayOfBirth(3);
    cout << endl;
    cout << "4: Expected: Wednesday, Actual: ";
    printDayOfBirth(4);
    cout << endl;
}

void test_determineDayOfBirth()
{
  // cout << "Now testing determinDayOfBirth() " << endl;
  // cout << "Please enter
}

void test_print10LeapYears()
{
 //   cout << "Now testing print10LeapYears()" << endl;
   // cout << "2000"
}
