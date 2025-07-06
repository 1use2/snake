#ifndef __POINT_HPP__
#define __POINT_HPP__

class Point
{
public:
    Point(int x = 0,int y = 0);
    int getX() const;
    int getY() const;
    void setPoint(int x,int y);
    void setX(int x);
    void setY(int y);

private:
    int _x;
    int _y;
};

#endif