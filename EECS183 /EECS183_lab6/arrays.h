#ifndef arrays_h
#define arrays_h

/**
 * arrays.h
 *
 * EECS 183
 * Lab 7: Array exercises
 *
 * Ellen Chlachidze
 * ellench
 *
 * <#A description of the lab here#>
 */

using namespace std;

//////////////////////////////////////////////////////////////////////
// STAFF-IMPLEMENTED UTILITY FUNCTIONS                              //
// -------------------------------------                            //
// We have implemented this function for you in arrays.cpp          //
//////////////////////////////////////////////////////////////////////

/**
 * Requires: size > 0 and size is the number of elements in arr
 * Modifies: cout
 * Effects : Prints the elements in the array arr, delimited by curly
 *           braces and separated by commas.
 */
void print_array(const int arr[], int size);

//////////////////////////////////////////////////////////////////////
// STUDENT-IMPLEMENTED UTILITY FUNCTIONS                            //
// -------------------------------------                            //
// You will need to implement these in arrays.cpp                   //
//////////////////////////////////////////////////////////////////////

/**
 * Requires: size > 0 and size is the number of elements in arr
 * Modifies: arr
 * Effects : Negates each element in arr.
 */
void negate_all(int arr[], int size);

/**
 * Requires: size > 0 and size is the number of elements in arr
 * Modifies: Nothing.
 * Effects : Returns true if the elements in arr are in order from
 *           least to greatest, with duplicates allowed.
 */
bool is_sorted(const int arr[], int size);

/**
 * Requires: size > 0 and size is the number of elements in source
 *           and target
 * Modifies: target
 * Effects : First, initializes all values in target to 0. 
 *           Then, copies the positive elements from source into adjacent
 *           locations in target.
 */
void copy_positive_elements(const int source[], int target[], int size);

/**
 * Requires: size > 0 and size is the number of elements in arr
 * Modifies: arr
 * Effects : Sorts elements of arr from least to greatest.
 */
void sort_array(int arr[], int size);

#endif
