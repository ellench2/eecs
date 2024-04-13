/**
 * Circle.cpp
 * Project UID 2e6ea4e086ea6a06753e819c30923369
 *
 * EECS 183
 * Project 4: CoolPics
 *
 * <#Name(s)#>
 * <#uniqname(s)#>
 *
 * <#Description#>
 */

#include "Circle.h"
#include "Line.h"
#include "Graphics.h"
#include "utility.h"
#include <algorithm>
using namespace std;

// TODO: implement two constructors, setCenter, getCenter, setColor, getColor,
//       setRadius, getRadius, read, write.
Circle::Circle() {
    Point pt1;
    center = pt1;
    radius = 0;
    Color black;
    color = black;
}
Circle::Circle(Point pt, int r, Color c) {
    center = pt1;
    radius = r;
    color = c;
}
void Circle::setCenter(Point pt) {
    center = pt1;
}
Point Circle::getCenter() {
    return center;
}
void Circle::setRadius(int r) {
    radius = r;
}
int Circle::getRadius() {
    return radius;
}
void Circle::setColor(Color c) {
    color = c;
}
Color Circle::getColor() {
    return color;
}
void Circle::read(istream& ins) {
    if (ins.good()) {
        ins >> center >> radius >> color;
    }
}
void Circle::write(ostream& outs) {
    if (outs.good()) {
        outs << center << " " << radius << " " << color;
    }
}

// Your code goes above this line.
// Don't change the implementations below!

istream& operator >> (istream& ins, Circle& circle)
{
    circle.read(ins);
    return ins;
}

ostream& operator << (ostream& outs, Circle circle)
{
    circle.write(outs);
    return outs;
}

void Circle::draw(Graphics & drawer)
{
    int radius = min(getRadius(), (int)DIMENSION);
    int error = -radius;
    int x = radius;
    int y = 0;
    Color c = getColor();

    while (x >= y)
    {
        plot8points(x, y, c, drawer);

        error += y;
        ++y;
        error += y;

        if (error >= 0)
        {
            error -= x;
            --x;
            error -= x;
        }
    }
}

int Circle::checkRadius(int radius)
{
    if (radius < 0)
    {
        return -1 * radius;
    }
    return radius;
}

void Circle::plot8points(int x, int y, Color c, Graphics& drawer)
{
    plot4points(x, y, c, drawer);
    if (x != y) plot4points(y, x, c, drawer);
}

void Circle::plot4points(int x, int y, Color c, Graphics& drawer)
{
    // cx and cy denote the offset of the circle center from the origin.
    int cx = getCenter().getX();
    int cy = getCenter().getY();

    Point pt1Start(cx - x, cy + y);
    Point pt1End(cx + x, cy + y);
    Line line1(pt1Start, pt1End, c);
    line1.draw(drawer);

    Point pt2Start(cx - x, cy - y);
    Point pt2End(cx + x, cy - y);
    Line line2(pt2Start, pt2End, c);
    line2.draw(drawer);
}
