//
//  main.cpp
//  exam2practice
//
//  Created by Ellen Chlachidze on 11/9/21.
//

#include <iostream>
#include <string>
using namespace std;
int MAX_SIZE = 20;

void formZ(bool board[4][4], int size);

void formZ(bool board[4][4], int size) {
    for (int i = 0; i < 4; i++) {
        board[0][i] = true;
    }
    for (int i = 0; i < 4; i++) {
        board[4-1][i] = true;
    }
    for (int i = 1; i < 4 - 1; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == (4 - j - 1)) {
                board[i][j] = true;
            }
            else {
                board[i][j] = false;
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    bool board1[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
    formZ(board1, 4);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << board1[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}

