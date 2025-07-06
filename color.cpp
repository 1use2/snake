#include "color.hpp"

Color::Color(uchar r,uchar g,uchar b):_r(r),_g(g),_b(b)
{
    _color = _r << 16 | _g << 8 | _b;
}

Color::Color(uint color)
{
    _b = color;
    _g = color >> 8;
    _r = color >> 16;
    _color = color;
}

void Color::setColor(uint color)
{
    _b = color;
    _g = color >> 8;
    _r = color >> 16;
    _color = color;
}

void Color::setColor(const Color &color)
{
    *this = color;
}

uint Color::getColor() const
{
    return _color;
}