/**
 * test.cpp
 * Project UID 2e6ea4e086ea6a06753e819c30923369
 *
 * EECS 183
 * Project 4: CoolPics
 *
 * <#Name(s)#>
 * <#uniqname(s)#>
 *
 * Contains functions for testing classes in the project. 
 */

#include "Graphics.h"
#include "Circle.h"
#include "Color.h"
#include "Line.h"
#include "Point.h"
#include "Rectangle.h"
#include "Shape.h"
#include "Triangle.h"


#include <iostream>
#include <fstream>

using namespace std;


void test_Circle();
void test_Color();
void test_Line();
void test_Point();
void test_Rectangle();
void test_Triangle();

void startTests() {
    test_Point();
    test_Line();
    test_Circle();
    test_Color();
    test_Rectangle();
    test_Triangle();
    // call other test functions here
    
    return;
}

void test_Point() {
    // test of default constructor
    Point p1;
    cout << "Expected: (0,0), actual: " << p1 << endl;
    
    // test of the non-default constructor
    Point p2(3, 9);
    cout << "Expected: (3,9), actual: " << p2 << endl;

    // test of member function: setX()
    p1.setX(5);
    // test of member function: setY()
    p1.setY(3);

    // test of member functions getX() and getY()
    cout << "Expected: (5,3), actual: ";
    cout << "( " << p1.getX()
         << ", " << p1.getY()
         << " )" << endl;

    return;
}

void test_Line() {
    Line lin1;
    Point pt0(4, 5);
    cout << lin1.getStart() << " " << lin1.getEnd() << " " << lin1.getColor() << endl;
    lin1.setEnd(pt0);
    cout << lin1.getEnd() << endl;
    Point pt1(10, 5);
    Point pt2(3,9);
    Color col1(100, 100, 60);
    Line lin2(pt1, pt2, col1);
    cout << lin2.getStart() << " " << lin2.getEnd() << " " << lin2.getColor() << endl;
    ofstream outs;
    outs.open("blank.txt");
  //  lin2.write(outs);
    outs << "(5,5) (95,5) 255 10 0" << endl;
    ifstream ins;
    ins.open("blank.txt");
    if (ins.is_open()) {
        cout << "ins opened successfully" << endl;
    }
    lin2.read(ins);
    cout << lin2.getStart() << " " << lin2.getEnd() << " " << lin2.getColor() << endl;
}

void test_Circle() {
    Circle cir1;
    Point pt1(3, 3);
    Point pt2(5, 5);
    Color red(255,0,0);
    cout << cir1.getCenter() << " " << cir1.getCenter() << " " << cir1.getRadius() << endl;
    Circle cir2(pt1, 5, red);
    cout << cir2.getCenter() << " " << cir2.getCenter() << " " << cir2.getRadius() << endl;
    cir2.setRadius(-10);
    cout << cir2.getRadius();
    cir2.setCenter(pt2);
    cout << cir2.getCenter();
    
}

void test_Color() {
    Color black;
    cout << black.getRed() << " " << black.getBlue() << " " << black.getGreen() << endl;
    Color green(0, 255, 0);
    cout << green.getGreen() << endl;
    green.setBlue(20);
    cout << green.getBlue() << endl;
    black.setBlue(900);
    cout << black.getBlue();
}

void test_Rectangle() {
    Rectangle rec1;
    Color black;
    Color blue(0, 255, 0);
    Color blue1(10, 250, 20);
    Color blue2(20, 255, 30);
    Color blue3(0, 200, 100);
    Point p1(0, 10);
    Point p2(75, 80);
    Rectangle rec2(p1, p2, blue);
    Rectangle rec3(p1, p2, blue, blue1, blue2, blue3);
    cout << rec1.getEnd() << " " << rec1.getStart() << " " << rec1.getColorTopLeft() << endl;
    cout << rec2.getEnd() << " " << rec2.getStart() << " " << rec2.getColorBottomLeft() << endl;
    cout << rec3.getEnd() << " " << rec3.getStart() << " " << rec3.getColorTopRight() << endl;
    rec1.setColor(blue);
    rec3.setColorTopRight(black);
    cout << rec3.getColorTopRight() << endl;
    cout << rec1.getColorTopLeft();
    rec1.setStart(p1);
    cout << rec1.getStart();
    ofstream out;
    out.open("blank2.txt");
    rec1.write(out);
}

void test_Triangle() {
    Triangle tr1;
    Point pt1(0, 10);
    Point pt2(10, 0);
    Point pt3(15,15);
    Point pt4(11, 0);
    Color red(255, 0, 0);
    Triangle tr2(pt1, pt2, pt3, red);
    cout << tr2.getVertexOne();
    tr2.setVertexTwo(pt4);
    cout << tr2.getVertexTwo();
    cout << tr1.getVertexOne();
    cout << tr1.getVertexOneColor();
    tr1.setVertexOne(pt1);
    cout << tr1.getVertexOne();
}
