/**
 * test.cpp
 * Project UID 5557cbe19b6ff413629a3743c503275413233136
 *
 * EECS 183
 * Project 3: 0h h1 Test Suite
 *
 * <#Name(s)#>
 * <#uniqname(s)#>
 *
 * <#A description of the project here#>
 */

#include <iostream>
#include "utility.h"
#include "ohhi.h"

void test_count_unknown_squares();
void test_check_valid_input();
void test_row_has_no_threes();
void test_cols_are_different();
void test_board_has_no_duplicates();
void test_solve_three_row();
void test_solve_three_col();
void test_solve_balance_row();
void test_check_valid_input();
void test_check_valid_move();

// declare more test functions here

void startTests() {
    test_count_unknown_squares();
    test_check_valid_input();
    test_row_has_no_threes();
    test_cols_are_different();
    test_board_has_no_duplicates();
    test_solve_three_row();
    test_solve_three_col();
    test_solve_balance_row();
    test_check_valid_move();
}


void test_count_unknown_squares() {
    int board[MAX_SIZE][MAX_SIZE];

    // test case 1
    string test_board_1[] = {"XOXO",
                             "--OO",
                             "---X",
                             "--O-"};
    int size_1 = 4;
    read_board_from_string(board, test_board_1, size_1);
    cout << count_unknown_squares(board, size_1) << endl;
    string test_board_2[] = {"----", "----", "----", "----"};
    int size_2 = 4;
    read_board_from_string(board, test_board_2, size_2);
    cout << count_unknown_squares(board, size_2) << endl;
}

void test_check_valid_input() {
    cout << "Testing check_valid_input" << endl;
    cout << "- - - -" << endl << "X X O -" << endl << "- X O -" << endl << "- - X -" << endl;
    cout << "row 2, column d, color red" << endl;
    int row = 0;
    int col = 0;
    cout << "Expecting 1, actual: " << check_valid_input(4, 2, 'd', 'X', row, col) << endl;
    cout << "Expecting row = 1, col = 3, actual row = " << row << ", actual col = " << col << endl;
}

void test_row_has_no_threes() {
    int board[MAX_SIZE][MAX_SIZE];

    cout << "Testing row_has_no_threes_of_color" << endl;
    string test_board_1[] = {"XXXO",
                             "--OO",
                             "---X",
                             "--O-"};
    int size_1 = 4;
    read_board_from_string(board, test_board_1, size_1);
    cout << "Expecting 0, actual: " << row_has_no_threes_of_color(board, size_1, 0, 1) << endl;
    string test_board_2[] = {"XXXO",
                             "--OO",
                             "---X",
                             "--O-"};
    int size_2 = 4;
    read_board_from_string(board, test_board_2, size_2);
    cout << "Expecting 1, actual: " << row_has_no_threes_of_color(board, size_2, 0, 2) << endl;
    
}

void test_cols_are_different() {
    cout << "Testing cols_are_different" << endl;
    int board[MAX_SIZE][MAX_SIZE];
    string test_board_1[] = {"XXXO",
                             "--OO",
                             "---X",
                             "--O-"};
    int size_1 = 4;
    read_board_from_string(board, test_board_1, size_1);
    cout << "Expecting 1, actual: " << cols_are_different(board, size_1, 0, 1) << endl;
    string test_board_2[] = {"XXXO",
                             "OOOO",
                             "XX-X",
                             "OOO-"};
    int size_2 = 4;
    read_board_from_string(board, test_board_2, size_2);
    cout << "Expecting 0, actual: " << cols_are_different(board, size_2, 0, 1) << endl;
}

void test_board_has_no_duplicates() {
    cout << "Testing board_has_no_duplicates" << endl;
    int board[MAX_SIZE][MAX_SIZE];
    string test_board_1[] = {"XXXO",
                             "--OO",
                             "---X",
                             "--O-"};
    int size_1 = 4;
    read_board_from_string(board, test_board_1, size_1);
    cout << "Expecting 1, actual: " << board_has_no_duplicates(board, size_1) << endl;
    string test_board_2[] = {"XXOO",
                             "XXOO",
                             "---X",
                             "--O-"};
    int size_2 = 4;
    read_board_from_string(board, test_board_2, size_2);
    cout << "Expecting 0, actual: " << board_has_no_duplicates(board, size_2) << endl;
    string test_board_3[] = {"XXXO",
                             "-OOO",
                             "-OOX",
                             "-XX-"};
    int size_3 = 4;
    read_board_from_string(board, test_board_3, size_3);
    cout << "Expecting 0, actual: " << board_has_no_duplicates(board, size_3) << endl;
}

void test_solve_three_row() {
    cout << "Testing solve_three_in_a_row" << endl;
    int board[MAX_SIZE][MAX_SIZE];
    string test_board_1[] = {"XOXO",
                             "--OO",
                             "----",
                             "--O-"};
    int size_1 = 4;
    read_board_from_string(board, test_board_1, size_1);
    cout << "Expecting: marking (2, B) as X; actual: " << endl;
    solve_three_in_a_row(board, size_1, 1, true);
    
}

void test_solve_three_col()  {
    cout << "Testing solve_three_in_a_col" << endl;
    int board[MAX_SIZE][MAX_SIZE];
    string test_board_1[] = {"XOXO",
                             "--OO",
                             "----",
                             "--O-"};
    int size_1 = 4;
    read_board_from_string(board, test_board_1, size_1);
    cout << "Expecting: marking (3, D) as X; actual: " << endl;
    solve_three_in_a_column(board, size_1, 3, true);
    
}

void test_solve_balance_row() {
    cout << "Testing solve_balance_row" << endl;
    int board[MAX_SIZE][MAX_SIZE];
    string test_board_1[] = {"XOXO",
                             "--OO",
                             "----",
                             "--O-"};
    int size_1 = 4;
    read_board_from_string(board, test_board_1, size_1);
    cout << "Expecting: marking (2, A) as X, marking (2, B) as X, actual: " << endl;
    solve_balance_row(board, size_1, 1, true);
}

void test_check_valid_move() {
    cout << "Testing check_valid_move" << endl;
    int orig_board[MAX_SIZE][MAX_SIZE];
    int curr_board[MAX_SIZE][MAX_SIZE];
    string test_board_1[] = {"XOXO",
                             "--O-",
                             "----",
                             "--O-"};
    string test_board_2[] ={"XOXO",
                            "--OO",
                            "----",
                            "--O-"};
    int size = 4;
    read_board_from_string(orig_board, test_board_1, size);
    read_board_from_string(curr_board, test_board_2, size);
    cout << "Expecting 'Sorry, that move violates a rule' and 0, actual: " << check_valid_move(orig_board, curr_board, size, 2, 3, 2) << endl;
    cout << "Expecting 'Sorry, original squares cannot be changed' and 0, actual: " << check_valid_move(orig_board, curr_board, size, 1, 2, 1) << endl;
}
