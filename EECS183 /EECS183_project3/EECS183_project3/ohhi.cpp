/**
 * ohhi.cpp
 * Project UID 5557cbe19b6ff413629a3743c503275413233136
 *
 * EECS 183
 * Project 3: 0h h1
 *
 * Ellen Chlachidze
 * ellench
 *
 * <#A description of the project here#>
 */

#include <iostream>
#include <cctype>
#include "utility.h"
#include "ohhi.h"

/**
 * --------------------------------------------------------
 * ---------- UTILITY FUNCTIONS ---------------------------
 * --------------------------------------------------------
 */

int count_unknown_squares(const int board[MAX_SIZE][MAX_SIZE], int size) {
    // write your code here
    int unknowns = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == UNKNOWN) {
                unknowns++;
            }
        }
    }
    return unknowns;
}


/**
 * --------------------------------------------------------
 * --------- VALIDITY CHECKING FUNCTIONS ------------------
 * --------------------------------------------------------
 */

bool row_has_no_threes_of_color(const int board[MAX_SIZE][MAX_SIZE],
                                int size,
                                int row,
                                int color) {
    bool no_run = true;
    for (int i = 0; i < size - 2; i++) {
        if (board[row][i] == board[row][i+1] && board[row][i+1] == board[row][i+2] && board[row][i] == color) {
            no_run = false;
        }
    }

    return no_run;
}

bool col_has_no_threes_of_color(const int board[MAX_SIZE][MAX_SIZE],
                                int size,
                                int col,
                                int color) {
    bool no_run = true;
    for (int i = 0; i < size - 2; i++) {
        if (board[i][col] == board[i+1][col] && board[i+1][col] == board[i+2][col] && board[i][col] == color) {
            no_run = false;
        }
    }
    return no_run;
}

bool board_has_no_threes(const int board[MAX_SIZE][MAX_SIZE], int size) {
    bool no_threes = true;
    for (int i = 0; i < size; i++) {
        if (!row_has_no_threes_of_color(board, size, i, RED)) {
            no_threes = false;
        }
    }
    for (int i = 0; i < size; i++) {
        if (!row_has_no_threes_of_color(board, size, i, BLUE)) {
            no_threes = false;
        }
    }
    for (int i = 0; i < size; i++) {
        if (!col_has_no_threes_of_color(board, size, i, BLUE)) {
            no_threes = false;
        }
    }
    for (int i = 0; i < size; i++) {
        if (!col_has_no_threes_of_color(board, size, i, RED)) {
            no_threes = false;
        }
    }

    return no_threes;
}

bool rows_are_different(const int board[MAX_SIZE][MAX_SIZE],
                        int size,
                        int row1,
                        int row2) {
    bool dif_rows = false;
    int dif = 0;
    int unknowns = 0;
    for (int i = 0; i < size; i++)   {
        if (board[row1][i] == UNKNOWN) {
            unknowns++;
        }
    }
    for (int i = 0; i < size; i++)   {
        if (board[row2][i] == UNKNOWN) {
            unknowns++;
        }
    }
    if (unknowns == 0) {
        for (int i = 0; i < size; i++) {
            if (board[row1][i] != board[row2][i]) {
                dif++;
            }
        }
    }
    if (unknowns > 0 || dif > 0) {
        dif_rows = true;
    }
    
    return dif_rows;
}

bool cols_are_different(const int board[MAX_SIZE][MAX_SIZE],
                        int size,
                        int col1,
                        int col2) {
    bool dif_cols = false;
    int dif = 0;
    int unknowns = 0;
    for (int i = 0; i < size; i++) {
        if (board[i][col1] == UNKNOWN) {
            unknowns++;
        }
    }
    for (int i = 0; i < size; i++) {
        if (board[i][col2] == UNKNOWN) {
            unknowns++;
        }
    }
    if (unknowns == 0) {
        for (int i = 0; i < size; i++) {
            if (board[i][col1] != board[i][col2]) {
                dif++;
            }
        }
    }
    if (unknowns > 0 || dif > 0) {
        dif_cols = true;
    }
    return dif_cols;
}

bool board_has_no_duplicates(const int board[MAX_SIZE][MAX_SIZE], int size) {
    bool duplicates = false;
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (!rows_are_different(board, size, i, j)) {
                duplicates = true;
            }
        }
    }
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (!cols_are_different(board, size, i, j)) {
                duplicates = true;
            }
        }
    }
    return !duplicates;
}


/**
 * --------------------------------------------------------
 * ---------- SOLVING FUNCTIONS ---------------------------
 * --------------------------------------------------------
 */

void solve_three_in_a_row(int board[MAX_SIZE][MAX_SIZE],
                          int size,
                          int row,
                          bool announce) {
    for (int i = 0; i < size - 2; i++) {
        if (board[row][i+1] != UNKNOWN && board[row][i+1] == board[row][i+2] && board[row][i] == UNKNOWN) {
            mark_square_as(board, size, row, i, opposite_color(board[row][i+1]), announce);
        }
        if (board[row][i] != UNKNOWN && board[row][i+1] == UNKNOWN && board[row][i] == board[row][i+2]) {
            mark_square_as(board, size, row, i+1, opposite_color(board[row][i]), announce);
        }
        if (board[row][i] != UNKNOWN && board[row][i] == board[row][i+1] && board[row][i+2] == UNKNOWN) {
            mark_square_as(board, size, row, i+2, opposite_color(board[row][i]), announce);
        }
        if (i < size - 3 && board[row][i] == UNKNOWN && board[row][i+3] == UNKNOWN && board[row][i+1] != UNKNOWN && board[row][i+1] == board[row][i+2]) {
            mark_square_as(board, size, row, i, opposite_color(board[row][i+1]), announce);
            mark_square_as(board, size, row, i+3, opposite_color(board[row][i+1]), announce);
        }
    }

        return;
}

void solve_three_in_a_column(int board[MAX_SIZE][MAX_SIZE],
                             int size,
                             int col,
                             bool announce) {
    for (int i = 0; i < size - 2; i++) {
        if (board[i+1][col] != UNKNOWN && board[i+1][col] == board[i+2][col] && board[i][col] == UNKNOWN) {
            mark_square_as(board, size, i, col, opposite_color(board[i+1][col]), announce);
        }
        if (board[i][col] != UNKNOWN && board[i+1][col] == UNKNOWN && board[i][col] == board[i+2][col]) {
            mark_square_as(board, size, i+1, col, opposite_color(board[i][col]), announce);
        }
        if (board[i][col] != UNKNOWN && board[i][col] == board[i+1][col] && board[i+2][col] == UNKNOWN) {
            mark_square_as(board, size, i+2, col, opposite_color(board[i][col]), announce);
        }
        if (i < size - 3 && board[i][col] == UNKNOWN && board[i+3][col] == UNKNOWN && board[i+1][col] != UNKNOWN && board[i+1][col] == board[i+2][col]) {
            mark_square_as(board, size, i, col, opposite_color(board[i+1][col]), announce);
            mark_square_as(board, size, i+3, col, opposite_color(board[i+1][col]), announce);
        }
    }
    return;
}


void solve_balance_row(int board[MAX_SIZE][MAX_SIZE],
                       int size,
                       int row,
                       bool announce) {
    int num_red = 0;
    int num_blue = 0;
    for (int i = 0; i < size; i++) {
        if (board[row][i] == RED) {
                num_red++;
        }
     }
    for (int i = 0; i < size; i++) {
        if (board[row][i] == BLUE) {
                num_blue++;
        }
     }
    if (num_red == size / 2) {
        for (int i = 0; i < size; i++)
            if (board[row][i] == UNKNOWN) {
                mark_square_as(board, size, row, i, BLUE, announce);
                }
    }
    if (num_blue == size / 2) {
        for (int i = 0; i < size; i++)
            if (board[row][i] == UNKNOWN) {
                mark_square_as(board, size, row, i, RED, announce);
                }
    }
    


    return;
}

void solve_balance_column(int board[MAX_SIZE][MAX_SIZE],
                          int size,
                          int col,
                          bool announce) {
    int num_red = 0;
    int num_blue = 0;
    for (int i = 0; i < size; i++) {
        if (board[i][col] == RED) {
                num_red++;
        }
     }
    for (int i = 0; i < size; i++) {
        if (board[i][col] == BLUE) {
                num_blue++;
        }
     }
    if (num_red == size / 2) {
        for (int i = 0; i < size; i++)
            if (board[i][col] == UNKNOWN) {
                mark_square_as(board, size, i, col, BLUE, announce);
                }
    }
    if (num_blue == size / 2) {
        for (int i = 0; i < size; i++)
            if (board[i][col] == UNKNOWN) {
                mark_square_as(board, size, i, col, RED, announce);
                }
    }
    


    return;
}


/**
 * --------------------------------------------------------
 * ---------- GAMEPLAY FUNCTIONS --------------------------
 * --------------------------------------------------------
 */

bool board_is_solved(const int board[MAX_SIZE][MAX_SIZE], int size) {
    int unknowns = count_unknown_squares(board, size);
    bool valid = board_is_valid(board, size);
    if (unknowns == 0 && valid) {
        return true;
    }
    else {
        return false;}
}

bool check_valid_input(int size, int row_input, char col_input,
                       char color_char, int &row, int &col) {
    bool valid = true;
    if (row_input < 1 || row_input > size) {
        valid = false;
    }
    if (col_input < 'A' || col_input > ('A' + size - 1)) {
        char new_col_input = toupper(col_input);
        if (new_col_input < 'A' || new_col_input > ('A' + size - 1)) {
            valid = false;
        }
    }
    if (color_char != RED_LETTER && color_char != BLUE_LETTER && color_char != UNKNOWN_LETTER) {
        char new_color_char = toupper(color_char);
        if (new_color_char != RED_LETTER && new_color_char != BLUE_LETTER && new_color_char != UNKNOWN_LETTER) {
            valid = false;
        }
    }
    if (!valid) {
        cout << "Sorry, that's not a valid input." << endl;
    }
    else {
        row = row_input - 1;
        if (col_input < '[') {
            col = col_input - 'A';
        }
        else {col = col_input - 'a';}
    }

    return valid;
}

bool check_valid_move(const int original_board[MAX_SIZE][MAX_SIZE],
                      const int current_board[MAX_SIZE][MAX_SIZE],
                      int size, int row, int col, int color) {
    bool valid = true;
    int copied_board[MAX_SIZE][MAX_SIZE];
    if (original_board[row][col] != UNKNOWN) {
        valid = false;
        cout << "Sorry, original squares cannot be changed." << endl;
    }
    else {
        copy_board(current_board, copied_board, size);
        mark_square_as(copied_board, size, row, col, color, false);
        if (!board_is_valid(copied_board, size)) {
            valid = false;
            cout << "Sorry, that move violates a rule." << endl;
        }
    }

    return valid;
}
