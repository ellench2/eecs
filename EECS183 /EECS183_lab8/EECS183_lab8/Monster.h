/**
 * Monster.h
 *
 * EECS 183: Lab 10 - Exam 2 Review
 *
 */
 
#ifndef MONSTER_H_
#define MONSTER_H_

#include <iostream>
#include <string>
using namespace std;

class Monster {
private:
    // type of Monster, e.g., "FluffyKitten"
    string type;
    
    // amount of hitPoints, e.g., 1000
    int hitPoints;
    
public:
    /**
     * Requires: Nothing.
     * Modifies: type, hitPoints
     * Effects:  Default constructor. Sets hitPoints to 0,
     *           type to ""
    */
    Monster();
    
    /**
     * Requires: Nothing.
     * Modifies: Nothing.
     * Effects:  returns type
    */
    string getType();
    
    /**
     * Requires: Nothing.
     * Modifies: Nothing.
     * Effects:  returns hitPoints
    */
    int getHitPoints();
    
    /**
     * Requires: Nothing.
     * Modifies: type, hitPoints
     * Effects:  Sets hitPoints to points,
     *           type to newType
    */
    void setData(string newType, int points);
};

#endif
