/**
 * Monster.cpp
 *
 * EECS 183: Lab 10 - Exam 2 Review
 *
 * NOTE: You will NOT submit this file to the autograder
 */

#include "Monster.h"
#include <iostream>
#include <string>
using namespace std;

Monster::Monster() {
    type = "";
    hitPoints = 0;
    return;
}

string Monster::getType() {
    return type;
}

int Monster::getHitPoints() {
    return hitPoints;
}

void Monster::setData(string newType, int points) {
    type = newType;
    hitPoints = points;
    return;
}


    
