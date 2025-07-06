#ifndef __COLOR_HPP__
#define __COLOR_HPP__

using uchar = unsigned char;
using uint = unsigned int;

enum CommonColor
{   
    COLOR_GREEN = 0x0000FF00,
    COLOR_BLUE = 0x000000FF,
    COLOR_RED = 0x00FF0000,   
    COLOR_WHITE = 0x00FFFFFF,
    COLOR_BLACK = 0x00000000,
    COLOR_ORANGE = 0x00FFB90F,
    COLOR_PURPLE = 0x00A020F0,
    COLOR_GRAY = 0x00A6A6A6
};

class Color
{
public:
    Color(uchar r,uchar g,uchar b);
    Color(uint color);
    void setColor(uint color);
    void setColor(const Color &color);
    uint getColor() const;

private:
    uchar _r;
    uchar _g;
    uchar _b;
    uint _color;
};

#endif