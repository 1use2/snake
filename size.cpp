#include "size.hpp"

Size::Size(int w,int h):_w(w),_h(h){}

int Size::getW() const
{
    return _w;
}

int Size::getH() const
{
    return _h;
}

void Size::setW(int w)
{
    _w = w;
}

void Size::setH(int h)
{
    _h = h;
}

void Size::setSize(int w,int h)
{
    _w = w;
    _h = h;
}