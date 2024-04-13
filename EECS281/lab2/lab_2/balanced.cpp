// Project Identifier: 1CAEF3A0FEDD0DEC26BA9808C69D4D22A9962768
//  balanced.cpp
//  lab02
//
//  Created by Ellen Chlachidze on 9/23/22.
//

#include <stdio.h>
#include <iostream>
#include <vector>
#include <utility>
#include <deque>
#include <algorithm>
#include "xcode_redirect.hpp"
using namespace std;


int main() {
    deque<char> compare;
    deque<char> balancer;
    bool balance = true;
    char temp;
    char a = '{';
    char a_ = '}';
    char b = '(';
    char b_ = ')';
    char c = '[';
    char c_ = ']';
    while (std::cin >> temp) {
        if (temp == b) {
            balancer.push_back(temp);
            compare.push_back(b_);
        }
        if (temp == a) {
            balancer.push_back(temp);
            compare.push_back(a_);
        }
        if (temp == c) {
            balancer.push_back(temp);
            compare.push_back(c_);
        }
        if (!compare.empty()) {
            if (temp == compare.back()) {
                balancer.pop_back();
                compare.pop_back();
            }
        }
        else if (temp == a_ || temp == b_ || temp == c_) {
            balance = false;
            break;
        }
    }
    if (!compare.empty() || !balance) {
        std::cout << "Not balanced\n";
    }
    else {
        std::cout << "Balanced\n";
    }
    return 0;
}

