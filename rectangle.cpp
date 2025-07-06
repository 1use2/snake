#include "rectangle.hpp"

Rectangle::Rectangle(int x,int y,int w,int h,uint color,int bw,uint bcolor):
_point(x,y),_size(w,h),_full_color(color),_border_width(bw),_border_color(bcolor){}

int Rectangle::getX() const
{
    return _point.getX();
}

int Rectangle::getY() const
{
    return _point.getY();
}

void Rectangle::move(int x,int y)
{
    _point.setPoint(x,y);
}

int Rectangle::getW() const
{
    return _size.getW();
}

int Rectangle::getH() const
{
    return _size.getH();
}

void Rectangle::resize(int w,int h)
{
    _size.setSize(w,h);
}

uint Rectangle::getFullColor() const
{
    return _full_color.getColor();
}

uint Rectangle::getBorderColor() const
{
    return _border_color.getColor();
}

int Rectangle::getBorderWidth() const
{
    return _border_width;
}