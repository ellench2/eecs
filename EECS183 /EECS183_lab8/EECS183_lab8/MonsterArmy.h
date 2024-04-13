/**
 * MonsterArmy.h
 *
 * EECS 183: Lab 10 - Exam 2 Review
 *
 * NOTE: You will NOT submit this file to the autograder
 */
#ifndef MONSTERARMY_H_
#define MONSTERARMY_H_

#include "Monster.h"
#include <iostream>
#include <string>
using namespace std;

const int MAX_ARMY_SIZE = 100;

class MonsterArmy {
private:
    // army is an array of Monster
    Monster army[MAX_ARMY_SIZE];
    
    // currentSize is the number of Monster class instances 
    // previosly added to the army array
    int currentSize;
    
public:

    /**
     * Requires: Nothing.
     * Modifies: currentSize
     * Effects:  Default constructor
    **/
    MonsterArmy(); 

    /**
     * Requires: type is a non-empty string, points >= 0.
     * Modifies: currentSize, army
     * Effects:  if space is available in the army array, sets the data
     *           of the monster at position currentSize in the array
     *           using the monster type and hit points given.
     *           Returns true if a monster was modified and false
     *           otherwise. currentSize should correctly keep track
     *           of the number of monsters spawned.
     * Note: you may assume the array army has been
     *       initialized with currentSize number of monsters.
    **/
    bool spawnMonster(string type, int points);

    /**
     * Requires: type is a non-empty string
     * Modifies: nothing
     * Effects:  returns the sum of total hit points
     *           for the given Monster type
    **/
    int pointsByType(string type);
};

#endif
