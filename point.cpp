#include "point.hpp"

Point::Point(int x,int y):_x(x),_y(y){}

int Point::getX() const
{
    return _x;
}

int Point::getY() const
{
    return _y;
}

void Point::setPoint(int x,int y)
{
    _x = x;
    _y = y;
}

void Point::setX(int x)
{
    _x = x;
}

void Point::setY(int y)
{
    _y = y;
}