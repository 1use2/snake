#ifndef __RECTANGLE_HPP__
#define __RECTANGLE_HPP__

#include "color.hpp"
#include "point.hpp"
#include "size.hpp"
#include "screen.hpp"

class Rectangle
{
    friend class Screen;
public:
    Rectangle(int x,int y,int w,int h,uint color,int bw,uint bcolor);
    int getX() const;
    int getY() const;
    void move(int x,int y);
    int getW() const;
    int getH() const;
    void resize(int w,int h);
    uint getFullColor() const;
    uint getBorderColor() const;
    int getBorderWidth() const;

private:
    Point _point;   //矩形的位置(左上角的位置)
    Size _size; //矩形的大小
    Color _full_color;  //矩形的填充颜色
    int _border_width;  //矩形边框宽度
    Color _border_color;    //矩形边框颜色
};

#endif