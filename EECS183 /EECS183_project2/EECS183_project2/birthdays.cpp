/**
 * birthdays.cpp
 * Project UID: d27ac2f9d9ef51141f8207ca61bd9b22d96076f1
 * 
 * Ellen Chlachidze
 * ellench
 *
 * EECS 183: Project 2
 *
 * <#description#>
 */

#include <cmath>
#include <iostream>
#include <string>

using namespace std;


/**
 * REQUIRES: nothing
 * MODIFIES: cout
 * EFFECTS:  Prints out the initial heading for the program
 */
void printHeading();


/**
 * REQUIRES: nothing
 * MODIFIES: cout
 * EFFECTS:  Prints out the final greeting for the program
 */
void printCloser();


/**
 * REQUIRES: nothing
 * MODIFIES: cout 
 * EFFECTS:  Prints the menu text. Does not read from cin.
 */
void printMenu();

/**
 * Note:  write your test cases in test.cpp BEFORE you implement this function
 *
 * REQUIRES: nothing
 * MODIFIES: cout, cin
 * EFFECTS:  1. Prints the menu
 *           2. Reads the input from the user
 *           3. Checks to make sure the input is within range for the menu
 *              If not prints "Invalid menu choice"
 *           4. Continues to print the menu and read an input until 
 *              a valid one is entered
 *           5. Returns the user's choice of menu options
 * 
 *           This function must call printMenu().
 */
int getMenuChoice();

/**
 * Note:  write your test cases in test.cpp BEFORE you implement this function
 *
 * REQUIRES: month, day, year represent a valid date
 * MODIFIES: nothing
 * EFFECTS:  Returns true if the date is in the limits
 *           of the Gregorian calendar otherwise returns false
 *           See the spec for definition of "limits of the Gregorian calendar"
 */
bool isGregorianDate(int month, int day, int year);

/**
 * Note:  write your test cases in test.cpp BEFORE you implement this function
 *
 * REQUIRES: year is a Gregorian year
 * MODIFIES: nothing
 * EFFECTS:  returns true if the year is a leap year
 *           otherwise returns false.
 *           See the spec for definition of a leap year.
 */
bool isLeapYear(int year);

/**
 * Note:  write your test cases in test.cpp BEFORE you implement this function
 *
 * REQUIRES: month, day, year may represent a date
 * MODIFIES: nothing
 * EFFECTS:  returns true if the date is valid
 *           otherwise returns false
 *           See the spec for definition of "valid"
 */
bool isValidDate(int month, int day, int year);

/**
 * Note:  write your test cases in test.cpp BEFORE you implement this function
 *
 * REQUIRES: month, day, year is a valid date
 *           i.e., the date passed to this function has already passed
 *               isValidDate()
 * MODIFIES: nothing
 * EFFECTS:  returns the value f that Zeller's formula calculates
 */
int determineDay(int month, int day, int year);

/**
 * Note:  write your test cases in test.cpp BEFORE you implement this function
 *
 * REQUIRES: day is a value of f output by Zeller's formula
*            (0 represents Saturday, 1 Sunday, 2 Monday, 3 Tuesday, etc)
 * MODIFIES: cout
 * EFFECTS:  prints the day of the week corresponding to day, suchas
 *           "Sunday", "Monday", ..., "Saturday"
 */
void printDayOfBirth(int day);

/**
 * Note:  write your test cases in test.cpp BEFORE you implement this function
 *
 * REQUIRES: nothing
 * MODIFIES: cout, cin
 * EFFECTS:  Asks for the month / day / year of the user's birthday.
 *           If the date is valid, it will print the day
 *           of the week you were born on
 *           Otherwise, it will print "Invalid date" prompt.
 */
void determineDayOfBirth();

/**
 * Note:  write your test cases in test.cpp BEFORE you implement this function
 *
 * REQUIRES: nothing
 * MODIFIES: cin, cout
 * EFFECTS:  Asks for a Gregorian year and prints the
 *           10 leap years after (not including) the year entered.
 *           If the year is invalid, it prints nothing.
 */
void print10LeapYears();

/* 
 * EFFECTS: Main driver for the Birthdays program. See the
 *          "Putting it Together" section of the spec for what
 *          this function needs to accomplish.
 */
void birthdays() {    
    // TODO: implement
    printHeading();
    int choice = getMenuChoice();
    while (choice != 3) {
        
    if (choice == 1){
        determineDayOfBirth();
    }
    else if (choice == 2){
        print10LeapYears();
    }
        choice = getMenuChoice();
    }
    if (choice == 3){
        printCloser();
    }
    
}


void printHeading() {
    cout << "*******************************" << endl
         << "      Birthday Calculator      " << endl
         << "*******************************" << endl << endl;
    return;
}


void printCloser() {
    cout << endl;
    cout << "****************************************************" << endl
         << "      Thanks for using the Birthday Calculator      " << endl
         << "****************************************************" << endl
         << endl;
    return;
}


void printMenu() {
    cout << endl;
    cout << "Menu Options" << endl
         << "------------" << endl;
    cout << "1) Determine day of birth" << endl;
    cout << "2) Print the next 10 leap years" << endl;
    cout << "3) Finished" << endl << endl;
    
    cout << "Choice --> ";
    return;
}

int getMenuChoice() {
    printMenu();
    int choice = 0;
    cin >> choice;
    cout << endl;
    while (choice < 1 || choice > 3) {
        cout << endl;
        cout << "Invalid menu choice";
        cout << endl;
        printMenu();
        cin >> choice;}
    return choice;
    // returning 0 to avoid compile error
    // return 0;
}

bool isGregorianDate(int month, int day, int year) {
    if (year > 1752) {
        return true;
    }
    else if (year == 1752 && month >= 10) {
        return true;
    }
    else if (year == 1752 && month == 9 && day >= 14) {
        return true;
    }
    else return false;
}

bool isLeapYear(int year) {
    if (year % 4 == 0) {
        if (year % 100 == 0) {
            if (year % 400 == 0) {
                return true;}
            else return false;
            }
        else return true;
        }
    else return false;
    }


bool isValidDate(int month, int day, int year) {
    if (isGregorianDate(month, day, year)) {
        if (month <= 12 && month > 0){
        if (day == 29 && month == 2 && isLeapYear(year)) {
            return true;
        }
        else if ((day == 30 || day == 29) && (month == 4 || month == 6 || month == 9 || month == 11)) {
            return true;
        }
        else if ((day == 31 || day == 30 || day == 29) && (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)) {
            return true;
        }
        else if (day < 29 && day > 0) {
            return true;
        }
        else {return false;}
        }
        else {return false;}
    }
    else {return false;}
}


int determineDay(int month, int day, int year) {
    int M = month;
    if (month < 3 && month > 0) {
        M = month + 12;
        year --;
    }
    int Y = year % 100;
    int C = year / 100 ;
    int f = (day + 13 * (M + 1) / 5 + Y + Y/4 + C/4 + 5 * C) % 7;
    return f;
}


void printDayOfBirth(int day) {
    if (day == 0) {
        cout << "Saturday" << endl;
    }
    if (day == 1) {
        cout << "Sunday" << endl;
    }
    if (day == 2) {
        cout << "Monday" << endl;
    }
    if (day == 3) {
        cout << "Tuesday" << endl;
    }
    if (day == 4) {
        cout << "Wednesday" << endl;
    }
    if (day == 5) {
        cout << "Thursday" << endl;
    }
    if (day == 6) {
        cout << "Friday" << endl;
    }
    
    return;
}


void determineDayOfBirth() {
    cout << "Enter your date of birth" << endl;
    cout << "format: month / day / year  -->";
    int month;
    int day;
    int year;
    char temp1;
    char temp2;
    cin >> month;
    cin >> temp1;
    cin >> day;
    cin >> temp2;
    cin >> year;
    cout << endl << endl;
    if (isValidDate(month, day, year) == 0) {
        cout << "Invalid Date" << endl << endl;
        return;
    }
    cout << "You were born on a: ";
    printDayOfBirth(determineDay(month, day, year));
    cout << endl;
    cout << "Have a great birthday!!!" << endl;
    return;
}

void print10LeapYears() {
    cout << "Enter year --> ";
    int year;
    int leaps = 0;
    cin >> year;
    cout << endl;
    if (year >= 1752) {
        if (year % 4 == 0) {
            year += 4;
        }
        while (leaps < 10) {
            if (isLeapYear(year)){
                cout << "Leap year is " << year << endl;
                leaps++;
            }
            year ++;
        }

    }
}
