/**
 * Point.cpp
 *
 * EECS 183
 * Lab 7
 * Taken from Project 4: CoolPics
 *
 * <#Name#>
 * <#uniqname#>
 *
 * <#Description#>
 */

#include "Point.h"

// for the declaration of DIMENSION
#include "utility.h"

// TODO: implement two constructors, setX, getX, setY, getY, read, write, checkRange.
int Point::checkRange(int val) {
    bool good = (val >= 0 && val < DIMENSION);
    if (!good) {
        if (val < 0) {
            val = 0;
        }
        else val = (DIMENSION - 1);
        
    }
    return val;
}

Point::Point() {
    x = 0;
    y = 0;
}
    
Point::Point(int xVal, int yVal) {
    x = checkRange(xVal);
    y = checkRange(yVal);
}

void Point::setX(int xVal) {
    x = checkRange(xVal);

    return;
}

int Point::getX() {
    return x;
}    

void Point::setY(int yVal) {
    y = checkRange(yVal);
    
    return;
}
    
int Point::getY() {
    return y;
}

void Point::read(istream& ins) {
    if (ins.good()) {
        char c = 'a';
        ins >> c;
        ins >> x;
        ins >> c;
        ins >> y;
    }
    return;
}    

void Point::write(ostream& outs) {
    x = checkRange(x);
    y = checkRange(y);
    if (outs.good()) {
        outs << "(" << x << "," << y << ")";
    }

    return;
}



// Your code goes above this line!!!
// Don't change the implementations below!!!

istream& operator >> (istream& ins,  Point& pt)
{
    pt.read(ins);
    return ins;
}

ostream& operator<< (ostream& outs, Point pt)
{
    pt.write(outs);
    return outs;
}

