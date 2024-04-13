/**
 * monsters.cpp
 *
 * <#Name#>
 * <#Uniqname#>
 *
 * EECS 183: Lab 10 - Exam 2 Practice
 *
 * Function to utilize your MonsterArmy.cpp implementations.
 * We have stubbed the required functions for you.
 *
 * NOTE: You WILL submit this file to the autograder
 */

#include "Monster.h"
#include "MonsterArmy.h"
#include <iostream>
#include <string>
using namespace std;

/**
 * Requires: size > 0
 * Modifies: army, ins
 * Effects:  extracts type and points for each monster from the ins
 *           input stream. The number of monsters to enter is given by
 *           the size parameter.
 */
void buildArmy(MonsterArmy & army, int size, istream & ins) {
    string type;
    int hitPoints = 0;
    int count = 0; 
    while (count < size && ins >> type >> hitPoints) {
        if (army.spawnMonster(type, hitPoints)) {
            count++;
        } else {
            return;
        }
    }
    return;
}

void monsters() {
    // TODO: replace the comments below with statements by filling in the blanks
    // See lab sepcification for sample runs of this function
	
    // __________ armySize = 5;
	
    // __________ << "Enter army size (>0): ";
	
    // __________ >> __________
	
    // __________ army;
	
    // buildArmy(__________, __________ , cin );
	
    // __________ totalHitPoints = army.__________("demogorgon");
	
    // __________ << "Your army of darkness is " << __________;
	
    cout << " demogorgon hit points strong!" << endl;	
    
    return;
}
